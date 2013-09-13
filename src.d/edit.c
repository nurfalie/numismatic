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
  char *id = NULL;
  char *tmp = NULL;
  char itmp[10];
  char field[MAX_1];
  char indata[MAX_1];
  char querystr[MAX_2];
  char *user_name = NULL;
  MYSQL_RES *res_set = NULL;
  MYSQL_ROW row = NULL;
  unsigned int i = 0;

  if(argc < 1)
    {
      (void) printf("Content-type: text/html\n\n");
      (void) printf("<html>\n");
      (void) printf("<title>Numismatic - Edit</title>\n");
      (void) printf("<body bgcolor=\"white\">\n");
      (void) printf("<center>%sError processing event.%s<br><br></center>\n",
		    FBEG, FEND);
      (void) printf("<center>\n");
      (void) printf("<a href=\"javascript:history.go(-1)\">%sBack%s</a> | ",
		    FBEG, FEND);
      (void) printf("<a href=\"javascript:window.close()\">%sClose%s</a>\n",
		    FBEG, FEND);
      (void) printf("</center>\n");
      (void) printf("</body></html>");
      return EXIT_FAILURE;
    }

  (void) printf("Content-type: text/html\n\n");
  (void) printf("<html>\n");
  (void) printf("<title>Numismatic - Edit</title>\n");
  (void) printf("<body bgcolor=\"white\">\n");

  if((connection = mysql_init(NULL)) == NULL)
    {
      (void) printf("<center>%sConnection error.%s<br><br>"
		    "</center>\n",
		    FBEG, FEND);
      (void) printf("<center>\n");
      (void) printf("<a href=\"javascript:history.go(-1)\">%sBack%s</a> | ",
		    FBEG, FEND);
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
      (void) printf("<center>%sConnection error.%s<br><br>"
		    "</center>\n",
		    FBEG, FEND);
      (void) printf("<center>\n");
      (void) printf("<a href=\"javascript:history.go(-1)\">%sBack%s</a> | ",
		    FBEG, FEND);
      (void) printf("<a href=\"javascript:window.close()\">%sClose%s</a>\n",
		    FBEG, FEND);
      (void) printf("</center>\n");
      (void) printf("</body></html>");
      return EXIT_FAILURE;
    }

  if(argc > 1)
    {
      /*
      ** Load and display the row information.
      */

      id = strtok(argv[1], "\\?");
      user_name = strtok(NULL, "\\?");

      if(id == NULL)
	id = "?";
      else if(user_name == NULL)
	user_name = "unknown";

      (void) memset(querystr, 0, sizeof(querystr));
      (void) snprintf(querystr,
		      sizeof(querystr),
		      "SELECT name, country, grade, "
		      "IFNULL(description, ''), "
		      "composition, weight, year, face_val, "
		      "acq_val, market_val, quantity, id FROM coin "
		      "WHERE id = %s AND user_name = '%s'",
		      id, user_name);

      if(mysql_query(connection, querystr) == 0)
	{
	  res_set = mysql_store_result(connection);
	  if(res_set == NULL)
	    {
	      /*
	      ** Entry missing.
	      */

	      (void) printf("<center>%sEntry missing.%s<br><br></center>\n",
			    FBEG, FEND);
	    }
	  else
	    {
	      /*
	      ** Process the query.
	      */

	      if((row = mysql_fetch_row(res_set)) != NULL)
		{
		  (void) printf("<form action=\"/cgi-bin/numismatic/"
				"edit.cgi\" "
				"method=\"post\">\n");
		  (void) printf("%s\n", FBEG);
		  (void) printf("<input type=\"hidden\" value=\"%s\" "
				"name=\"id\">\n",
				argv[1]);
		  (void) printf("<font color=\"red\">Name</font>"
				"<br><input type=\"text\" size=64 "
				"maxlength=64 "
				"name=\"name\" "
				"value=\"%s\"><br>\n", row[0]);
		  (void) printf("Country<br>");
		  (void) printf("<select name=country size=1>");

		  for(i = 0; i < sizeof(COUNTRIES) / sizeof(COUNTRIES[0]); i++)
		    if(strcmp(row[1], COUNTRIES[i]) == 0)
		      (void) printf("<option selected>%s\n", COUNTRIES[i]);
		    else
		      (void) printf("<option>%s\n", COUNTRIES[i]);

		  (void) printf("</select><br>\n");
		  (void) printf("Grade<br>");
		  (void) printf("<select name=grade size=1>");

		  for(i = 0; i < sizeof(GRADES) / sizeof(GRADES[0]); i++)
		    if(strcmp(row[2], GRADES[i]) == 0)
		      (void) printf("<option selected>%s\n", GRADES[i]);
		    else
		      (void) printf("<option>%s\n", GRADES[i]);

		  (void) printf("</select><br>\n");
		  (void) printf("Description<br>");
		  (void) printf("<textarea wrap=physical name=\"description\" "
				"cols=32 rows=4>%s"
				"</textarea><br>",
				(strlen(row[3]) == 0) ? "" : row[3]);
		  (void) printf("<font color=\"red\">Composition"
				"</font>"
				"<br><input type=\"text\" size=32 "
				"name=\"composition\" "
				"maxlength=32 value=\"%s\"><br>\n", row[4]);
		  (void) printf("<font color=\"red\">Weight</font>"
				"<br><input type=\"text\" size=32 "
				"maxlength=32 "
				"name=\"weight\" "
				"value=\"%s\"><br>\n", row[5]);
		  (void) printf("Year<br>");
		  (void) printf("<select name=year size=1>");

		  if(strcmp(row[6], "N/A") == 0)
		    (void) printf("<option selected>N/A");
		  else
		    (void) printf("<option>N/A");

		  for(i = YEAR_START; i <= YEAR_END; i++)
		    {
		      (void) memset(itmp, 0, sizeof(itmp));
		      (void) snprintf(itmp, sizeof(itmp), "%ud", i);

		      if(strcmp(row[6], itmp) == 0)
			(void) printf("<option selected>%s", itmp);
		      else
			(void) printf("<option>%s", itmp);
		    }

		  (void) printf("</select><br>\n");
		  (void) printf("<font color=\"red\">Face Value"
				"</font> (Numeric)"
				"<br><input type=\"text\" size=10 "
				"maxlength=10 name=\"face_val\" "
				"value=\"%s\"><br>\n", row[7]);
		  (void) printf("<font color=\"red\">"
				"Acquisition Value</font> (Numeric)"
				"<br><input type=\"text\" "
				"size=10 "
				"name=\"acq_val\" "
				"maxlength=10 value=\"%s\"><br>\n", row[8]);
		  (void) printf("<font color=\"red\">"
				"Market Value</font> (Numeric)"
				"<br><input type=\"text\" size=10 "
				"name=\"market_val\" "
				"maxlength=10 value=\"%s\"><br>\n", row[9]);
		  (void) printf("Quantity<br>");
		  (void) printf("<select name=quantity size=1>");

		  for(i = 1; i <= MAX_QUAN; i++)
		    {
		      (void) memset(itmp, 0, sizeof(itmp));
		      (void) snprintf(itmp, sizeof(itmp), "%ud", i);

		      if(strcmp(row[10], itmp) == 0)
			(void) printf("<option selected>%s", itmp);
		      else
			(void) printf("<option>%s", itmp);
		    }

		  (void) printf("</select><br>\n");
		  (void) printf("<center>\n");
		  (void) printf("<input type=\"submit\" value=\"Save "
				"Information\">"
				"\n");
		  (void) printf("</center>\n");
		  (void) printf("%s\n", FEND);
		  (void) printf("</form>\n");

		  /*
		  ** Free some space.
		  */
		}
	      else
		{
		  (void) printf("<center>%sEntry missing.%s<br><br>"
				"</center>\n",
				FBEG, FEND);
		}

	      mysql_free_result(res_set);
	    }
	}
      else
	{
	  (void) printf("<center>%sFetch error.%s<br><br></center>\n",
			FBEG, FEND);
	}
    }
  else
    {
      (void) memset(indata, 0, sizeof(indata));

      if(fgets(indata, (int) sizeof(indata) - 1, stdin) == NULL)
	{
	  goto error;
	}

      id = tmp = strtok(indata, "&");

      if(id == NULL)
	{
	  goto error;
	}

      (void) memset(field, 0, sizeof(field));
      (void) memset(querystr, 0, sizeof(querystr));
      (void) strncpy
	(querystr, "UPDATE coin SET ",
	 fmin(sizeof(querystr) - strlen(querystr) - 1,
	      strlen("UPDATE coin SET ")));

      while(tmp != NULL)
	{
	  tmp = strtok(NULL, "&");

	  if(tmp != NULL)
	    {
	      if(i++ <= 6)
		{
		  update(field, tmp, sizeof(field), EDIT);
		}
	      else
		{
		  (void) memset(field, 0, sizeof(field));
		  (void) strncpy
		    (field, tmp, fmin(sizeof(field) - strlen(field) - 1,
				      strlen(tmp)));
		}

	      (void) strncat
		(querystr, field,
		 fmin(sizeof(querystr) - strlen(querystr) - 1,
		      strlen(field)));
	      (void) strncat
		(querystr, ", ",
		 fmin(sizeof(querystr) - strlen(querystr) - 1,
		      strlen(", ")));
	    }
	}

      querystr[strlen(querystr) - 2] = '\0';
      (void) strncat
	(querystr, " WHERE ", fmin(sizeof(querystr) - strlen(querystr) - 1,
				   strlen(" WHERE ")));
      (void) strncat
	(querystr, id, fmin(sizeof(querystr) - strlen(querystr) - 1,
			    strlen(id)));

    error:

      if(mysql_query(connection, querystr) != 0)
	{
	  (void) printf("<center>%sUpdate error.<br>"
			"Please be sure to complete all required "
			"fields.%s<br><br></center>\n",
			FBEG, FEND);
	}
      else
	{
	  (void) printf("<center>%sInformation saved.%s<br><br>"
			"</center>\n", FBEG, FEND);
	  (void) printf("<script>"
			"if(this.window != null && "
			"   this.window.opener != null && "
			"   !this.window.opener.closed)"
			"this.window.opener.refresh();</script>\n");
	}
    }

  (void) printf("<center>\n");
  (void) printf("<a href=\"javascript:history.go(-1)\">%sBack%s</a> | ",
		FBEG, FEND);
  (void) printf("<a href=\"javascript:window.close()\">%sClose%s</a>\n",
		FBEG, FEND);
  (void) printf("</center>\n");
  mysql_close(connection);
  (void) printf("</body></html>");
  return EXIT_SUCCESS;
}
