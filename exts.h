#ifndef EXTS_H
#define EXTS_H

#define AR_EC RED
static char *archiveexts[] = {
    ".zip",
    ".gz",
    ".bz2",
    ".7z",
    ".rar",
    ".xz",
    ".tar",
    ".z",
    ".tar.xz",
    ".tar.gz",
    ".tar.bz2",
    ".tgz",
    ".txz",
    (char *) NULL
};

#define IMG_EC CYAN
static char *imgexts[] = {
  ".png",
  ".jpe",
  ".jpg",
  ".jpeg",
  ".tiff",
  ".gif",
  ".bmp",
  ".webp",
  (char *) NULL
};

#define CODE_EC GREEN
static char *code[] = {
  ".c",
  ".cpp",
  ".h",
  ".hpp",
  ".go",
  ".rs",
  ".mod",
  ".lisp",
  ".scm",
  ".o",
  ".out",
  ".sh",
  ".html",
  ".css",
  ".js",
  (char *) NULL
};

#define MD_EC MAGENTA
static char *mediaexts[] = {
  ".mp4",
  ".ogg",
  ".ogv",
  ".mkv",
  ".mp3",
  ".aac",
  ".flac",
  ".webm",
  (char *)NULL
};

#define DOC_EC YELLOW
static char *docexts[] = {
  ".txt",
  ".log",
  ".pdf",
  ".doc",
  ".docx",
  ".ps",
  ".epub",
  ".org",
  (char *)NULL
};

#endif
