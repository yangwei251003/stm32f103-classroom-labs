"""Restore independently obtained external SPL/CMSIS files; never downloads or republishes them."""
from pathlib import Path
import argparse,hashlib,json,os,shutil
ROOT=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,required=True,help='Existing original classroom folder or independently obtained STSW-STM32054 3.5.0 extraction')
    ap.add_argument('--allow-compatible-mismatch',action='store_true',help='Explicitly accept differing SDK bytes after your own version/license review; software validation no longer matches the archived SDK')
    args=ap.parse_args();src=args.source.resolve()
    if not src.is_dir():ap.error('source directory does not exist')
    manifest=json.loads((ROOT/'dependencies/stm32f10x-files.json').read_text(encoding='utf-8'))
    names={Path(x['external_path']).name for x in manifest['files']};found={n:[] for n in names}
    for parent,dirs,files in os.walk(src):
        dirs[:]=[x for x in dirs if x not in ['.git','node_modules','Objects','Listings','.metadata']]
        for name in names.intersection(files):found[name].append(Path(parent)/name)
    selected=[]
    for item in manifest['files']:
        candidates=found[Path(item['external_path']).name]
        exact=next((p for p in candidates if sha(p)==item['sha256']),None)
        chosen=exact or (candidates[0] if args.allow_compatible_mismatch and len(candidates)==1 else None)
        if chosen is None:ap.error('missing, ambiguous or non-matching dependency: '+item['external_path']+'; no files copied')
        selected.append((item,chosen))
    target=(ROOT/'external/STM32F10x').resolve()
    assert target.is_relative_to(ROOT.resolve())
    records=[]
    for item,source in selected:
        p=target/item['external_path'];p.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,p)
        records.append({'path':item['external_path'],'sha256':sha(p),'matches_archive':sha(p)==item['sha256']})
    (target/'restoration-receipt.json').write_text(json.dumps({'files':records,'all_match_archive':all(x['matches_archive'] for x in records)},indent=2),encoding='utf-8')
    print(json.dumps({'files_restored':len(records),'all_match_archive':all(x['matches_archive'] for x in records),'output':'external/STM32F10x; ignored by Git'}))
if __name__=='__main__':main()
