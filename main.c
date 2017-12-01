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
#include <stdlib.h>
#include <time.h>

#define LINESIZE 700
#define TITLESIZE 300


struct FileType {
    char pretty_title[TITLESIZE];
    char machine_title[TITLESIZE];
};

struct FileType *file_from_title(char *title);

int titlecmp(char *line1, char *line2, size_t longer_line) {
    for (int i = 0; i < longer_line-1; i++) {
        if (line1[i] != line2[i])
            return 1;
    }
    return 0;
}

int post_exists(struct FileType *post, FILE *database) {
    FILE *start = database;
    size_t longer_line;
    char line[LINESIZE];

    while (fgets(line, sizeof(line), start)) {
        // get longest line
        if (strlen(line)-1>strlen(post->pretty_title))
            longer_line = strlen(line)-1;
        else
            longer_line = strlen(post->pretty_title);

        if (titlecmp(post->pretty_title, line, longer_line) == 0)
            return 1;
        else continue;
    }
    return 0;
}

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

void machine_title(char *dest, char *title) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char date_string[26];

    strftime(date_string, 26, "%Y-%m-%d-", tm); // with trailing dash

    /// name part
    char title_string[strlen(title)+1];
    for (int i = 0; i <= strlen(title); i++) {
        if (title[i] == ' ')
            title_string[i] = '-';
        else
            title_string[i] = title[i];
    }
    title_string[strlen(title)] = '\0';

    strcpy(dest, date_string);
    strcpy(dest + strlen(date_string), title_string);
    strcpy(dest + strlen(date_string) + strlen(title_string), ".markdown");
}

void append_head_matter(struct FileType *post, FILE *new_post) {

    char head_matter[500];
    strcpy(head_matter, "---\n");
    strcpy(head_matter + 4, "title: \"");
    strcpy(head_matter + 4 + 8, post->pretty_title);
    strcpy(head_matter + 4 + 8 + strlen(post->pretty_title), "\"\n");
    strcpy(head_matter + 4 + 8 + strlen(post->pretty_title) + 2, "layout: post\n---\n\n");

    if (new_post) {
        fputs(head_matter, new_post);
        fclose(new_post);
    }
    else {
        printf("couldn't create new post.\n");
        exit(EXIT_FAILURE);
    }
}

struct FileType *file_from_title(char *title) {

    struct FileType *post = malloc(sizeof(*post));
    machine_title(post->machine_title, title);
    strcpy(post->pretty_title, title);

    /// create the new file

    return post;
}

void open(struct FileType *post) {
    char command[1024];
    strcpy(command, "emacs ");
    strcpy(command + 6, post->machine_title);
    strcpy(command + strlen(post->machine_title) + 6, " &");

    FILE *editor = popen(command, "r");
    printf("Opening...\n");

    if (!editor)
        exit(EXIT_FAILURE);

    printf("Exiting...\n");
    pclose(editor);
}

struct FileType *new_post(char *title, FILE *database) {
    /*!
     * 1. check if file already exists
     * 2. use current date / time and title string to create file
     *      * date format: "YYYY-MM-DD-your-title-here.markdown"
     */

    struct FileType *post = file_from_title(title);

    if (post_exists(post, database)) {
        printf("Post already exists\n");
        open(post);
        fclose(database);
        exit(EXIT_SUCCESS);
    }
    else {
        FILE *new_post = fopen(post->machine_title, "a+");
        append_head_matter(post, new_post);
        return post;
    }
}

int add_to_database(struct FileType *file, FILE *database) {
    printf("Adding file to database\n");
    char mapping[600];
    strcpy(mapping, file->pretty_title);
    strcpy(mapping + strlen(file->pretty_title), "\n");
    strcpy(mapping + strlen(file->pretty_title) + 1, file->machine_title);
    strcpy(mapping + strlen(file->pretty_title) + strlen(file->machine_title) + 1, "\n\n");
    fputs(mapping, database);

    printf("Added \"");
    printf(file->pretty_title);
    printf("\"\n");
}

int main( int argc, char *argv[] ) {
    /*!
     *
     */

    FILE *database = fopen(".projekts", "a+");

    if (!database) {
        printf("There's an issue with the database file (file pointer is null)\n");
        return 1;
    }

    if (argc >= 3) {
        // has to be new post
        if (strcmp(argv[1], "post") == 0) {
            // everything after the "post" keyword is treated as the post title
            char title[256];
            int length = 0;

            for (int i = 2, j = 0; i < argc; i++) {
                strcpy(title + j, argv[i]);
                j += strlen(argv[i]);
                title[j++] = ' ';
                length = j;
            }

            title[length-1] = '\0';
            struct FileType *file = new_post(title, database);
            add_to_database(file, database);
            fclose(database);
            open(file);
            exit(EXIT_SUCCESS);
        }
        else {
            printf("%s is not a supported command.\n", argv[1]);
            usage();
            return 1;
        }
    }
}