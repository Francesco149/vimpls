#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

void tsend(int f, char* s) {
  for (; *s; ioctl(f, TIOCSTI, s++));
}

int readall(int f, char** buf, int* buf_len) {
  int n = 0;
  do {
    int r;
    char* new_buf;
    if (!*buf_len) {
      *buf_len = 16;
    } else {
      *buf_len *= 2;
    }
    new_buf = realloc(*buf, *buf_len);
    if (!new_buf) {
      free(*buf);
      return 0;
    }
    r = read(f, new_buf + n, *buf_len - 1);
    if (r == -1) {
      free(new_buf);
      return 0;
    }
    *buf = new_buf;
    n += r;
  }
  while (n == *buf_len - 1);
  (*buf)[n] = 0;
  return 1;
}

int vimpls() {
  char* buf;
  char* cmd = 0;
  int cmd_len = 0;
  struct dirent* dir;
  DIR* d;
  d = opendir("/proc/");
  if (!d) {
    return 0;
  }
  buf = malloc(512);
  if (!readall(0, &cmd, &cmd_len)) {
    return 0;
  }
  while ((dir = readdir(d))) {
    int f, res;
    if (strlen("/proc//cmdline") + strlen(dir->d_name) >= 512) {
      continue;
    }
    *buf = 0;
    strcat(buf, "/proc/");
    strcat(buf, dir->d_name);
    strcat(buf, "/cmdline");
    f = open(buf, O_RDONLY);
    if (f == -1) {
      continue;
    }
    res = read(f, buf, 511);
    close(f);
    if (res == -1) {
      continue;
    }
    buf[res] = 0;
    if (strstr(buf, "vim") == buf) {
      *buf = 0;
      strcat(buf, "/proc/");
      strcat(buf, dir->d_name);
      strcat(buf, "/fd/0");
      f = open(buf, O_RDWR);
      if (f == -1) {
        continue;
      }
      tsend(f, cmd);
      close(f);
    }
  }
  closedir(d);
  free(buf);
  free(cmd);
  (void)cmd;
  return 1;
}

int main(int argc, char* argv[]) {
  (void)argv;
  if (argc != 1) {
    char* usage = (
      "broadcasts a command to all running vim instances\n\n"
      "usage: vimpls commands ...\n"
      "example: printf \"\\033:wqa\\!\\n\" | sudo vimpls\n"
    );
    write(2, usage, strlen(usage));
    return 1;
  }
  if (!vimpls()) {
    return 1;
  }
  return 0;
}
