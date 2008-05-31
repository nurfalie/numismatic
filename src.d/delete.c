/*
** -- Local Includes --
*/

#include "common.h"

/*
** -- MySQL Includes --
*/

#include <mysql/mysql.h>

static MYSQL *connection = NULL;

int main(int argc, char *argv[])
{
  char *tmp = NULL;
  char querystr[MAX_2];

  (void) printf("Content-type: text/html\n\n");
  (void) printf("<html>\n");
  (void) printf("<title>Numismatic - Delete</title>\n");
  (void) printf("<body bgcolor=\"white\">\n");

  if((connection = mysql_init(NULL)) == NULL)
    {
      (void) printf("<center>%sInitialization error.%s<br><br></center>\n",
		    FBEG, FEND);
      (void) printf("<center>\n");
      (void) printf("<a href=\"javascript:window.close()\">%sClose%s</a>\n",
		    FBEG, FEND);
      (void) printf("</center>\n");
      (void) printf("</body></html>");
      return EXIT_FAILURE;
    }

  if(mysql_real_connect(connection,
			MYHOST,
			MYUSER,
			MYPASSWORD,
			MYDB,
			0,
			NULL,
			0) == NULL)
    {
      (void) printf("<center>%sConnection error.%s<br><br></center>\n",
		    FBEG, FEND);
      (void) printf("<center>\n");
      (void) printf("<a href=\"javascript:window.close()\">%sClose%s</a>\n",
		    FBEG, FEND);
      (void) printf("</center>\n");
      (void) printf("</body></html>");
      return EXIT_FAILURE;
    }

  if(argv[1] != NULL)
    {
      tmp = strtok(argv[1], ",");
      (void) printf("%s\n", FBEG);

      while(tmp != NULL)
	{
	  (void) memset(querystr, 0, sizeof(querystr));
	  (void) snprintf(querystr, sizeof(querystr),
			  "UPDATE coin SET id = -%s WHERE id = %s",
			  tmp, tmp);

	  if(mysql_query(connection, querystr) == 0)
	    {
	      (void) printf("Deleted ID %s.<br>\n", tmp);
	    }
	  else
	    {
	      (void) printf("Unable to delete ID %s.<br>\n", tmp);
	    }

	  tmp = strtok(NULL, ",");
	}

      (void) printf("%s\n", FEND);
      (void) printf("<script>"
		    "if(this.window != null && "
		    "   this.window.opener != null && "
		    "   !this.window.opener.closed)"
		    "this.window.opener.refresh();</script>\n");
    }

  (void) printf("<center>\n");
  (void) printf("<a href=\"javascript:window.close()\">%sClose%s</a>\n",
		FBEG, FEND);
  (void) printf("</center>\n");
  (void) printf("</body></html>");
  mysql_close(connection);
  return EXIT_SUCCESS;
}
