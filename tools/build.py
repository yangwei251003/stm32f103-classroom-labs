"""Compile and link the minimally corrected examples only. Never flashes or connects to a device."""
from pathlib import Path
import argparse,json,subprocess,os,re
ROOT=Path(__file__).resolve().parents[1]
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--toolchain',type=Path,required=True,help='ARM Compiler 6 bin directory containing armclang, armasm, armlink')
    ap.add_argument('--lab',choices=['led-key','oled-digits','all'],default='all')
    args=ap.parse_args();bin=args.toolchain.resolve();out=ROOT/'build';out.mkdir(exist_ok=True)
    # The archived CMSIS header predates armclang and does not define its inline macro for this compiler.
    flags=['--target=arm-arm-none-eabi','-mcpu=cortex-m3','-mthumb','-std=c99','-Os','-ffunction-sections','-fdata-sections','-D__INLINE=inline','-D__DSB()=__builtin_arm_dsb(0xF)','-DUSE_STDPERIPH_DRIVER','-DSTM32F10X_MD','-Iexamples/common','-Iexternal/STM32F10x/Library','-Iexternal/STM32F10x/Startup']
    suffix='.exe' if os.name=='nt' else ''
    jobs=[];layouts=[]
    def run(tool,arguments,label):
        result=subprocess.run([str(bin/(tool+suffix)),*arguments],cwd=ROOT,capture_output=True)
        (out/(label+'.log')).write_bytes(result.stdout+b'\n'+result.stderr)
        jobs.append({'job':label,'tool':tool,'exit_code':result.returncode})
        if result.returncode:
            print(json.dumps({'failed_job':label,'exit_code':result.returncode,'log':'build/'+label+'.log'}))
            (out/'validation.json').write_text(json.dumps({'jobs':jobs,'hardware_tested':False},indent=2),encoding='utf-8')
            raise SystemExit(result.returncode)
    for lab in ['led-key','oled-digits'] if args.lab=='all' else [args.lab]:
        sources=[('examples/'+lab+'/main.c','main'),('examples/common/delay.c','delay'),('external/STM32F10x/Library/stm32f10x_gpio.c','gpio'),('external/STM32F10x/Library/stm32f10x_rcc.c','rcc'),('external/STM32F10x/Startup/system_stm32f10x.c','system')]
        objects=[]
        for source,stem in sources:
            obj='build/'+lab+'-'+stem+'.o';run('armclang',[*flags,'-c',source,'-o',obj],lab+'-'+stem);objects.append(obj)
        startup='build/'+lab+'-startup.o'
        run('armasm',['--cpu','Cortex-M3','--pd','__MICROLIB SETA 1','external/STM32F10x/Startup/startup_stm32f10x_md.s','-o',startup],lab+'-startup');objects.append(startup)
        run('armlink',['--cpu','Cortex-M3','--library_type=microlib','--entry=Reset_Handler','--ro-base=0x08000000','--rw-base=0x20000000','--first=__Vectors','--map','--list=build/'+lab+'.map','--info=sizes','--output=build/'+lab+'.axf',*objects],lab+'-link')
        text=(out/(lab+'.map')).read_text(encoding='utf-8')
        regions=[]
        for name,base,size in re.findall(r'(?:Load|Execution) Region (\w+) \(Base: (0x[0-9a-fA-F]+), Size: (0x[0-9a-fA-F]+)',text):
            address,length=int(base,16),int(size,16)
            if 0x08000000<=address<0x10000000:assert address+length<=0x08010000,'exceeds original 64KiB flash target'
            elif 0x20000000<=address<0x30000000:assert address+length<=0x20005000,'exceeds original 20KiB RAM target'
            else:raise AssertionError('unexpected linked region')
            regions.append({'name':name,'base':base,'bytes':length})
        assert len(regions)>=3,'missing layout evidence'
        layouts.append({'lab':lab,'regions':regions,'rom_bytes':int(re.search(r'Total ROM Size[^\n]*?\)\s+(\d+)',text).group(1))})
    report={'jobs':jobs,'layouts':layouts,'hardware_tested':False,'original_classroom_projects_built':False,'examples_only':True}
    (out/'validation.json').write_text(json.dumps(report,indent=2),encoding='utf-8')
    print(json.dumps({'jobs':len(jobs),'all_exit_zero':True,'hardware_tested':False}))
if __name__=='__main__':main()
