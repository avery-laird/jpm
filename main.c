/*!
 * simple project manager for jekyll.
 *
 * usage: projekt post [postname] | ""
 *
 * -e  --editor=EDITOR (has to be a binary on the path, not a name)
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define LINESIZE 700

void list(FILE *database);
void usage();
int new_post(char *title);

void list(FILE *database) {
    /*!
     * parse every entry in the database file, output each
     * pretty title
     */
    char line[LINESIZE];
    char *long_title, *pretty_title;
    char *save_ptr = line;
    while (fgets(line, sizeof(line), database)) {
        // skip long title. Add a switch later to include long titles
        strtok_r(line, " ", &save_ptr);
        pretty_title = strtok_r(NULL, " ", &save_ptr);
        printf("%s\n", pretty_title);
    }
}

void usage() {
    printf("do this later\n");
}

int new_post(char *title) {
    // construct
    //create new file
    FILE *new_post = fopen()
}

int main( int argc, char *argv[] ) {
    /*!
     *
     */

    char current_directory[1024];

    if (getcwd(current_directory, sizeof(current_directory)) != NULL) {
          printf("Initializing new project...\n%s", current_directory);
    }
    else {
        printf("There was a problem getting the current directory.\n");
        return 1;
    }

    FILE *database = fopen(".projekt", "a+");

    if (!database) {
        printf("There's an issue with the database file (file pointer is null)\n");
        return 1;
    }

    if (argc == 2) {
        // only one argument, has to be list or init (init not supported yet)
        if (strcmp(argv[1], "list") == 0) {
            list(database);
            return 0;
        }
        else {
            printf("%s is not a supported command.\n", argv[1]);
            usage();
            return 1;
        }
    }
    else if (argc == 3) {
        // has to be new post
        if (strcmp(argv[1], "post") == 0) {
            // everything after the "post" keyword is treated as the post title
            char title[256]; title[255] = '\0';
            int length = 0, i = 0;
            while(title[length] != '\0' && length < 256 && i++ < argc) {
                strcpy(&title[length], argv[i]);
                length += strlen(argv[i]);
                title[length++] = ' ';
            }
            new_post(title);
            return 0;
        }
        else {
            printf("%s is not a supported command.\n", argv[1]);
            usage();
            return 1;
        }
    }

    /*if (access(".projekt", F_OK) != -1) {
        // a project already exists
        printf("A project already exists. Remove the current"
                       " .projekt file if you wish to create a fresh database.\n");
        return 0;
    }
    else {

    }*/

}