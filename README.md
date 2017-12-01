# Jekyll Post Manager

This is the repository for the simple Jekyll post manager as described in 
[my blog post](averylaird.com/2017/11/30/A-Simple-Jekyll-Post-Manager/).
To try it out, just:
 
1. Clone the repository

       $ git clone git@github.com:avery-laird/jpm.git

2. Compile `jpm.c`

       $ gcc jpm.c -o jpm
    
3. Move it to your `_posts` directory, and play around:

       $ ./jpm post This Is A New Post
    
Contributions are very welcome, just open a pull request.
Or, to make a suggestion, open an issue.

The running tab of todos, for those interested:

- [ ] Convert copied strings to formatted strings
- [ ] Consider changing `post` syntax to require titles be enclosed in quotations
- [ ] Write the `edit` command 
- [ ] Derive `list` from `edit`
- [ ] Parse YAML
- [ ] Convert relative paths to user-defined paths (with defaults)
- [ ] Add configuration file ability
- [ ] Support arbitrary editors