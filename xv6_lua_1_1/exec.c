#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "defs.h"
#include "x86.h"
#include "elf.h"

int
exec(char *path, char **argv)
{
  char *s, *last;
  int i, off;
  uint argc, sz, sp, ustack[3+MAXARG+1];
  struct elfhdr elf;
  struct inode *ip;
  struct proghdr ph;
  pde_t *pgdir, *oldpgdir;
  struct proc *curproc = myproc();

  //cprintf("path: %s \n", path);
  //lua path is   path: lua
  char lua_path[] = "lua";
  char lua_header[] = "--LUA 1.1";
  char lua_test[sizeof(lua_header)];
  char* script_path = (char*)0; //set to NULL
  int is_lua_script = 0;

  begin_op();
  if((ip = namei(path)) == 0){
    end_op();
    cprintf("exec: fail\n");
    return -1;
  }

  ilock(ip);
  pgdir = 0;

  //lua header check
  if (readi(ip, lua_test, 0, sizeof(lua_header - 1)) == sizeof(lua_header - 1)) //skip reading a null char ;) this tests to see if the file has at least this many bytes
  {
      if (strncmp(lua_header, lua_test, sizeof(lua_header - 1)) == 0) //actually see if it matches signiture exactly
      {
          cprintf("lua script detected\n");
          is_lua_script = 1;
          script_path = path; //save the old path which is the script
          path = lua_path; //swap lua programs path where the script path was
          cprintf("script path: %s \n", script_path);
          iunlockput(ip); //unlock script file
          end_op(); //end op on script file

          begin_op(); //repeat steps above but now we are using lua's executable instead :D
          ip = namei(path);
          ilock(ip); //don't need to assign pgdir to 0 because it was done before here
      }
  }

  // Check ELF header
  if(readi(ip, (char*)&elf, 0, sizeof(elf)) != sizeof(elf))
    goto bad;
  if(elf.magic != ELF_MAGIC)
    goto bad;

  if((pgdir = setupkvm()) == 0)
    goto bad;

  // Load program into memory.
  sz = 0;
  for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
    if(readi(ip, (char*)&ph, off, sizeof(ph)) != sizeof(ph))
      goto bad;
    if(ph.type != ELF_PROG_LOAD)
      continue;
    if(ph.memsz < ph.filesz)
      goto bad;
    if(ph.vaddr + ph.memsz < ph.vaddr)
      goto bad;
    if((sz = allocuvm(pgdir, sz, ph.vaddr + ph.memsz)) == 0)
      goto bad;
    if(ph.vaddr % PGSIZE != 0)
      goto bad;
    if(loaduvm(pgdir, (char*)ph.vaddr, ip, ph.off, ph.filesz) < 0)
      goto bad;
  }
  iunlockput(ip);
  end_op();
  ip = 0;

  // Allocate two pages at the next page boundary.
  // Make the first inaccessible.  Use the second as the user stack.
  sz = PGROUNDUP(sz);
  if((sz = allocuvm(pgdir, sz, sz + 2*PGSIZE)) == 0)
    goto bad;
  clearpteu(pgdir, (char*)(sz - 2*PGSIZE));
  sp = sz;

  if (is_lua_script == 0)
  {
      // Push argument strings, prepare rest of stack in ustack.
      for (argc = 0; argv[argc]; argc++) {
          if (argc >= MAXARG)
              goto bad;
          sp = (sp - (strlen(argv[argc]) + 1)) & ~3;
          if (copyout(pgdir, sp, argv[argc], strlen(argv[argc]) + 1) < 0)
              goto bad;
          ustack[3 + argc] = sp;
      }
  }
  else
  {
      //push lua path name and then the script name
      argc = 0; //push lua path
      sp = (sp - (strlen(lua_path) + 1)) & ~3;
      if (copyout(pgdir, sp, lua_path, strlen(lua_path) + 1) < 0)
          goto bad;
      ustack[3 + argc] = sp;
      //cprintf("argv[argc] %s \n", argv[argc]);

      argc++; //push script name
      sp = (sp - (strlen(script_path) + 1)) & ~3;
      if (copyout(pgdir, sp, script_path, strlen(script_path) + 1) < 0)
          goto bad;
      ustack[3 + argc] = sp;
      //cprintf("argv[argc] %s \n", argv[argc]);
      argc++;
  }
  ustack[3+argc] = 0;

  ustack[0] = 0xffffffff;  // fake return PC
  ustack[1] = argc;
  ustack[2] = sp - (argc+1)*4;  // argv pointer

  sp -= (3+argc+1) * 4;
  if(copyout(pgdir, sp, ustack, (3+argc+1)*4) < 0)
    goto bad;

  // Save program name for debugging.
  for(last=s=path; *s; s++)
    if(*s == '/')
      last = s+1;
  safestrcpy(curproc->name, last, sizeof(curproc->name));

  // Commit to the user image.
  oldpgdir = curproc->pgdir;
  curproc->pgdir = pgdir;
  curproc->sz = sz;
  curproc->tf->eip = elf.entry;  // main
  curproc->tf->esp = sp;
  switchuvm(curproc);
  freevm(oldpgdir);
  return 0;

 bad:
  if(pgdir)
    freevm(pgdir);
  if(ip){
    iunlockput(ip);
    end_op();
  }
  return -1;
}
