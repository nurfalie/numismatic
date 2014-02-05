/*
** -- Local Includes --
*/

#include "common.h"

/*
** -- MySQL Includes --
*/

#include <mysql/mysql.h>

static MYSQL *connection = 0;

int main(int argc, char *argv[])
{
  MYSQL_RES *res_set = 0;
  MYSQL_ROW row = 0;
  char query[MAX_2];
  int ct = 0;
  size_t i = 0;
  size_t j = 0;

  (void) printf("Content-type: text/html\n\n");
  (void) printf("<html>\n");
  (void) printf("<title>Numismatic - Print</title>\n");
  (void) printf("<body bgcolor=\"white\">\n");

  if(argc != 2)
    goto error;

  if((connection = mysql_init(0)) == 0)
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
			0,
			0) == 0)
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

  (void) memset(query, 0, sizeof(query));
  (void) snprintf(query,
		  sizeof(query),
		  "SELECT name, country, grade, "
		  "IF0(description, ''), "
		  "composition, weight, year, face_val, "
		  "acq_val, market_val, quantity, "
		  "quantity * market_val total "
		  "FROM coin "
		  "WHERE id > 0 AND user_name = '%s' ORDER BY name",
		  argv[1]);

  if(mysql_query(connection, query) == 0)
    {
      res_set = mysql_store_result(connection);

      if(res_set == 0)
	{
	  /*
	  ** We do not have entries.
	  */
	}
      else
	{
	  /*
	  ** Process the query.
	  */

	  ct = 0;
	  (void) printf("<table>");

	  while((row = mysql_fetch_row(res_set)) != 0)
	    {
	      ct += 1;
	      (void) printf("<td><table border>\n");
	      (void) printf("\n");

	      for(i = 0; i < mysql_num_fields(res_set); i++)
		{
		  (void) printf("<tr>"
				"<th bgcolor=\"#EEEEEE\"><font size=2>\n");

		  if(i == 0)
		    (void) printf("Name");
		  else if(i == 1)
		    (void) printf("Country");
		  else if(i == 2)
		    (void) printf("Grade");
		  else if(i == 3)
		    (void) printf("Description");
		  else if(i == 4)
		    (void) printf("Composition");
		  else if(i == 5)
		    (void) printf("Weight");
		  else if(i == 6)
		    (void) printf("Year");
		  else if(i == 7)
		    (void) printf("Face Value");
		  else if(i == 8)
		    (void) printf("Acquisition Value ($)");
		  else if(i == 9)
		    (void) printf("Market Value  ($)");
		  else if(i == 10)
		    (void) printf("Quantity");
		  else
		    (void) printf("Total ($)");

		  if(i == 3 && strlen(row[3]) > 0)
		    {
		      (void) printf("\n</font></th><th bgcolor=\"beige\">"
				    "<font size=2>");

		      for(j = 0; j < strlen(row[i]); j++)
			{
			  if((j + 1) % 32 == 0)
			    (void) printf("%c<br>", row[i][j]);
			  else
			    (void) printf("%c", row[i][j]);
			}

		      (void) printf("</font></th></tr>");
		    }
		  else
		    (void) printf("\n</font></th><th bgcolor=\"beige\">"
				  "<font size=2>%s</font></th></tr>",
				  (strlen(row[i]) == 0) ? "&nbsp" : row[i]);
		}

	      (void) printf("</table></td>");

	      if(ct % 2 == 0)
		(void) printf("<tr></tr>");
	    }

	  (void) printf("</table>");

	  /*
	  ** Free some space.
	  */

	  mysql_free_result(res_set);
	}
    }
  else
    {
      (void) printf("<center>%sFetch error.%s</center>\n", FBEG, FEND);
    }

  if(ct != 0)
    (void) printf("<br>");

 error:

  (void) printf("<center>\n");

  if(ct != 0)
    (void) printf("<a href=\"javascript:window.print()\">%sPrint%s</a>\n",
		  FBEG, FEND);

  (void) printf("<a href=\"javascript:window.close()\">%sClose%s</a>\n",
		FBEG, FEND);
  (void) printf("</center>\n");
  (void) printf("</body></html>");
  mysql_close(connection);
  return EXIT_SUCCESS;
}
