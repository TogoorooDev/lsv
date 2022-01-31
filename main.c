#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>
#include <pwd.h>
#include <grp.h>

#include "colors.h"
#include "exts.h"

int hidden;
int longform;
char *path;

void args(int argc, char *argv[]){
  hidden = 0;
  
  for (unsigned short i = 0; i < argc; i++){
    if (strcmp(argv[i], "-a") == 0) hidden = 1;
  }
}

int getclr(char *name){
  // search for ext
  char *extonly = name;
  
  extonly = (strrchr(extonly, '.'));
  if (extonly == NULL) return WHITE;
  
  //while (strchr(extonly, '.') != NULL) extonly = strchr(extonly, '.');
  for (size_t i = 0; i < strlen(extonly); i++){
    extonly[i] = tolower(extonly[i]);
  }
  
  for (int i = 0; archiveexts[i] != NULL; i++){
    if (strcmp(extonly, archiveexts[i]) == 0) return AR_EC;
  }
  
  for (int i = 0; imgexts[i] != NULL; i++){
    if (strcmp(extonly, imgexts[i]) == 0) return IMG_EC;
  }

  for (int i = 0; code[i] != NULL; i++){
    if (strcmp(extonly, code[i]) == 0) return CODE_EC;
  }
  
  for (int i = 0; mediaexts[i] != NULL; i++){
    if (strcmp(extonly, mediaexts[i]) == 0) return MD_EC;
  }

  for (int i = 0; docexts[i] != NULL; i++){
    if (strcmp(extonly, docexts[i]) == 0) return DOC_EC;
  }

  return WHITE;
}

char *calcspace(int size){
  double newsize;
  char ident = 'B';
  
  if (size >= 1073742000){
    newsize = size / 1073742000.0;
    ident = 'G';
  }else if (size >= 1048576){
    newsize = size / 1048576.0;
    ident = 'M';
  }else if (size >= 1024){
    newsize = size / 1024.0;
    ident = 'K';
  }else {
    newsize = size;
    ident = 'B';
  }

  char *ret = malloc(24);

  if (newsize == (int) newsize){
    sprintf(ret, "%g%c", newsize, ident);
  }else { 
    sprintf(ret, "%3.1f%c", newsize, ident);
  }
  
  return ret;
  
}

int printdir(char *name){
  struct stat data;
  char fullpath[2048];
  struct passwd *uinfo;
  struct group *ginfo;
  
  //colors
  char *clrname = malloc(512);
  char *clrowner = malloc(128);
  int clr;

  
  sprintf(fullpath, "%s/%s", path, name);

  if (stat(fullpath, &data) != 0) {
    sprintf(clrname, "\e[41m\e[6;37m%s\e[0m\n", name);

    return 0;
  }
  

  uinfo = getpwuid(data.st_uid);
  ginfo = getgrgid(data.st_gid);

  //printf("%s %s", uinfo->pw_name, ginfo->gr_name);

  sprintf(clrowner, "\e[0;3%dm%s %s\e[0m", YELLOW, uinfo->pw_name, ginfo->gr_name);
  
  char umode[48];
  char gmode[48];
  char amode[48];
  
  char mode[] = "----------";
	
  if (S_ISREG(data.st_mode))
    mode[0] = '-';
  else if (S_ISBLK(data.st_mode))
    mode[0] = 'b';
  else if (S_ISCHR(data.st_mode))
    mode[0] = 'c';
  else if (S_ISDIR(data.st_mode))
    mode[0] = 'd';
  else if (S_ISFIFO(data.st_mode))
    mode[0] = 'p';
  else if (S_ISLNK(data.st_mode))
    mode[0] = 'l';
  else if (S_ISSOCK(data.st_mode))
    mode[0] = 's';
  else
    mode[0] = '?';

  if (data.st_mode & S_IRUSR) mode[1] = 'r';
  if (data.st_mode & S_IWUSR) mode[2] = 'w';
  if (data.st_mode & S_IXUSR) mode[3] = 'x';
  if (data.st_mode & S_IRGRP) mode[4] = 'r';
  if (data.st_mode & S_IWGRP) mode[5] = 'w';
  if (data.st_mode & S_IXGRP) mode[6] = 'x';
  if (data.st_mode & S_IROTH) mode[7] = 'r';
  if (data.st_mode & S_IWOTH) mode[8] = 'w';
  if (data.st_mode & S_IXOTH) mode[9] = 'x';

  if (data.st_mode & S_ISUID) mode[3] = (mode[3] == 'x') ? 's' : 'S';
  if (data.st_mode & S_ISGID) mode[6] = (mode[6] == 'x') ? 's' : 'S';
  if (data.st_mode & S_ISVTX) mode[9] = (mode[9] == 'x') ? 't' : 'T';

  
  if (S_ISDIR(data.st_mode)){
    clr = BLUE;
  }else {
    clr = getclr(name);
  }

  if (mode[3] == 'x' && !(S_ISDIR(data.st_mode))) sprintf(clrname, "\e[3;3%dm%s\e[0m", clr, name);
  else if (S_ISDIR(data.st_mode)) sprintf(clrname, "\e[0;4%dm%s\e[0m", clr, name);
  else sprintf(clrname, "\e[0;3%dm%s\e[0m", clr, name);
  
  sprintf(umode, "\e[0;96m%c\e[0;91m%c\e[0;92m%c", mode[1], mode[2], mode[3]);
  sprintf(gmode, "\e[0;96m%c\e[0;91m%c\e[0;92m%c", mode[4], mode[5], mode[6]);
  sprintf(amode, "\e[0;96m%c\e[0;91m%c\e[0;92m%c", mode[7], mode[8], mode[9]);

  char exspacing[2];

  char sizechar[512];

  sprintf(sizechar, "%ld", data.st_size);
  
  if (strlen(sizechar) == 1){
    exspacing[0] = '\t';
    exspacing[1] = '\0';
  }else {
    exspacing[0] = '\0';
  }
  
  
  if (data.st_mode & S_IEXEC){
    printf("\e[0;94m%c\e[0m%s%s%s\e[0m %s %s%s\t %s\n", mode[0], umode, gmode, amode, clrowner, calcspace(data.st_size), exspacing, clrname);
  }else {
    printf("\e[0;94m%c\e[0m%s%s%s\e[0m %s %s%s\t %s\n", mode[0], umode, gmode, amode, clrowner, calcspace(data.st_size), exspacing, clrname);
  }
    
  return 0;
}

int cmpfunc(const void *a, const void *b){  
  const char *ia = *(const char **)a;
  const char *ib = *(const char **)b;
  
  return strcmp(ia, ib);
}

int main(int argc, char *argv[]){
  path = malloc(2048);
  char *entries[4096];
  DIR *dirp;
  struct dirent *de;
  
  args(argc, argv);
  
  getcwd(path, 2047);
  dirp = opendir(path);

  if (!dirp){
    puts("Open Error");
    return 1;
  }

  int count = 0;
  
  while ((de = readdir(dirp)) != NULL){
    if ((de->d_name[0] != '.') || ((hidden) && (strcmp(de->d_name, ".") != 0) && (strcmp(de->d_name, "..") != 0 ))){
      char *name = de->d_name;
      entries[count] = malloc(512);
      entries[count] = name;

      count++;
    }
  }

  entries[count] = NULL;

  qsort(entries, count, sizeof(char *), cmpfunc);
  
  for (int i = 0; entries[i] != NULL; i++){
    printdir(entries[i]);
  }
  
  closedir(dirp);
  free(path);
  
  return 0;
}
