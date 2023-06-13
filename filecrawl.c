# include <stdio.h>
# include <stdlib.h>
# include <dirent.h>
# include <unistd.h>
# include <pthread.h>

# define MAX_BUFFER 300
# define THREAD_COUNT 15

const char * blue = "\x1b[34m", * red = "\x1b[31m", * reset = "\x1b[0m";
pthread_t threads[THREAD_COUNT];

void * scanDirectory(int threadNumber);

int main(int argc, char * argv[]) {
    char * directory = (argc > 1 ? argv[1] : "./");
    printf("%sEntering %s%s\n\n", red, directory, reset);
    chdir(directory);
    scanDirectory(0);
    return 0;
}

void * scanDirectory(int threadNumber) {
  struct dirent * de;
  char directory[MAX_BUFFER], * currentFile;
  getcwd(directory, (int) MAX_BUFFER);
  DIR * dir = opendir(directory);
    for (int i = 0; ((de = readdir(dir)) != NULL); i++) {
      currentFile = de -> d_name;
       if (i < 2) { continue; };
       //The first two results are always ./ and ../
       if (de -> d_type == DT_DIR) {
          chdir(currentFile);
          printf("%s%s/%s\n%sEntering %s%s\n\n", blue, directory, currentFile, red, currentFile, reset);
          threadNumber += (threadNumber >= ((int) THREAD_COUNT) - 1 ? -threadNumber : 1);
          pthread_create(&threads[threadNumber], NULL, scanDirectory, threadNumber);
          pthread_join(threads[threadNumber], NULL);
       } else {
          printf("%s/%s\n", directory, currentFile);
       }
    }
    printf("%sExiting %s%s\n\n", red, directory, reset);
    closedir(dir);
    chdir("../");
    pthread_exit(NULL);
    return (void *) 1;
}