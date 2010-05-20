#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <dirent.h>

#define MMAXDIR 512

/**
	Recurse into the dname directory.
	\param f The name of the file that contains the file list
	\param buf The buffer contiaining the directory name.
*/
int r_dirname(FILE *f,FILE *d,const char *dname)
{
	DIR   *dir;
	struct dirent *direntry;
	char buf[MMAXDIR];

	/* get the directory structure */
	dir = opendir(dname);
	if (dir==NULL) {
		printf("failed to open the directory\n");
		return 0;
	}

	while (1) {
		/* read the directory entry*/
		direntry = readdir(dir);

		/* check if the last entry were reached */
		if (direntry == NULL) {
			break;
		}

		/* print the directory info */
		if (direntry->d_type == DT_REG) {
			fprintf(f,"%s/%s\n",dname,direntry->d_name);
		} else if (direntry->d_type == DT_DIR) {
			/* ignore directories . and .. */
			if (direntry->d_name[0] != '.') {
				sprintf(buf,"%s/%s",dname,direntry->d_name);
				fprintf(d,"%s\n",buf);
				r_dirname(f,d,buf);
				
			}
		}
	}

	/* close the directory */
	closedir(dir);

	return 1;
}

int fl_createlist(const char* dname, const char *ftargetname,const char *dtargetname)
{
	FILE *f,*d;

	/* open the target file */
	f = fopen(ftargetname,"wt");
	if (!f) {
		printf("failed to open [%s]\n",ftargetname);
		return 0;
	}

	/* open the directory target */
	d = fopen(dtargetname,"wt");
	if (!d) {
		printf("failed to open [%s]\n",dtargetname);
		fclose(f);
		return 0;
	}

	r_dirname(f,d,dname);

	/* close the target file */
	fclose(f);	fclose(d);

	return 1;
}
