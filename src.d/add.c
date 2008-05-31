/*
** Alexis Megas.
** (c) 2002, 2004, 2007.
**
** Date        Name                         Purpose
** 02/25/2007  Alexis Megas                 More descriptive labels.
*/

/*
** -- Local Includes --
*/

#include "common.h"

/*
** -- MySQL Includes --
*/

#include <mysql/mysql.h>

/*
** -- Local Globals --
*/

/*
** strlen("name=") = 5, but change to 5 + 1 = 6, etc.
*/

static MYSQL *connection = NULL;
static int sizes[] = {6, 9, 7, 13, 13, 8, 6, 10, 9, 12, 10};

int main(int argc, char *argv[])
{
  char *id = NULL;
  char *tmp = NULL;
  char itmp[10];
  char field[MAX_1];
  char indata[MAX_1];
  char querystr[MAX_2];
  char querystr1[MAX_2];
  char user_name[33];
  MYSQL_RES *res_set = NULL;
  MYSQL_ROW row = NULL;
  unsigned int i = 0;
  unsigned int j = 0;
  unsigned int k = 0;
  unsigned int error = 0;

  (void) printf("Content-type: text/html\n\n");
  (void) printf("<html>\n");
  (void) printf("<title>Numismatic - Add</title>\n");
  (void) printf("<body bgcolor=\"white\">\n");

  if(strstr(argv[1], "?") == NULL)
    {
      (void) printf("<form action=\"/cgi-bin/numismatic/add.cgi?%s?\" "
		    "method=\"post\">\n", argv[1]);
      (void) printf("%s\n", FBEG);
      (void) printf("<font color=\"red\">Name</font>"
		    "<br><input type=\"text\" size=64 "
		    "maxlength=64 "
		    "name=\"name\" "
		    "value=\"\"><br>\n");
      (void) printf("Country<br><select name=country size=1>");

      for(i = 0; i < sizeof(COUNTRIES) / sizeof(COUNTRIES[0]); i++)
	(void) printf("<option>%s\n", COUNTRIES[i]);

      (void) printf("</select><br>\n");
      (void) printf("Grade<br>");
      (void) printf("<select name=grade size=1>");

      for(i = 0; i < sizeof(GRADES) / sizeof(GRADES[0]); i++)
	(void) printf("<option>%s\n", GRADES[i]);

      (void) printf("</select><br>\n");
      (void) printf("Description<br>");
      (void) printf("<textarea wrap=physical name=\"description\" "
		    "cols=32 rows=4></textarea><br>");
      (void) printf("<font color=\"red\">Composition</font>"
		    "<br><input type=\"text\" size=32 "
		    "name=\"composition\" "
		    "maxlength=32 value=\"\"><br>\n");
      (void) printf("<font color=\"red\">Weight</font>"
		    "<br><input type=\"text\" size=32 "
		    "maxlength=32 "
		    "name=\"weight\" "
		    "value=\"\"><br>\n");
      (void) printf("Year<br>");
      (void) printf("<select name=year size=1>");
      (void) printf("<option>N/A");

      for(i = YEAR_START; i <= YEAR_END; i++)
	{
	  (void) memset(itmp, 0, sizeof(itmp));
	  (void) snprintf(itmp, sizeof(itmp), "%d", i);
	  (void) printf("<option>%s", itmp);
	}

      (void) printf("</select><br>\n");
      (void) printf("<font color=\"red\">Face Value</font> (Numeric)<br>"
		    "<input type=\"text\" size=10 "
		    "maxlength=10 name=\"face_val\" "
		    "value=\"\"><br>\n");
      (void) printf("<font color=\"red\">Acquisition Value</font> (Numeric)"
		    "<br><input type=\"text\" "
		    "size=10 "
		    "name=\"acq_val\" "
		    "maxlength=10 value=\"\"><br>\n");
      (void) printf("<font color=\"red\">Market Value</font> (Numeric)"
		    "<br><input type=\"text\" size=10 "
		    "name=\"market_val\" "
		    "maxlength=10 value=\"\"><br>\n");
      (void) printf("Quantity<br>");
      (void) printf("<select name=quantity size=1>");

      for(i = 1; i <= MAX_QUAN; i++)
	{
	  (void) memset(itmp, 0, sizeof(itmp));
	  (void) snprintf(itmp, sizeof(itmp), "%d", i);
	  (void) printf("<option>%s", itmp);
	}

      (void) printf("</select><br>\n");
      (void) printf("<center>\n");
      (void) printf("<input type=\"submit\" value=\"Save Information\">\n");
      (void) printf("</center>\n");
      (void) printf("%s\n", FEND);
      (void) printf("</form>\n");
    }
  else
    {
      if((connection = mysql_init(NULL)) == NULL)
	{
	  (void) printf("<center>%sInitialization error.%s<br><br>"
			"</center>\n",
			FBEG, FEND);
	  (void) printf("<center>\n");
	  (void) printf("<a href=\"javascript:history.go(-1)\">%"
			"sBack%s</a> | ",
			FBEG, FEND);
	  (void) printf("<a href=\"javascript:window.close()\">%"
			"sClose%s</a>\n",
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
	  (void) printf("<a href=\"javascript:history.go(-1)\">%"
			"sBack%s</a> | ",
			FBEG, FEND);
	  (void) printf("<a href=\"javascript:window.close()\">%"
			"sClose%s</a>\n",
			FBEG, FEND);
	  (void) printf("</center>\n");
	  (void) printf("</body></html>");
	  return EXIT_FAILURE;
	}

      (void) memset(user_name, '\0', sizeof(user_name));

      /*
      ** -2 => ganoof\?, we need to remove the \?.
      */

      (void) strncpy(user_name, argv[1], strlen(argv[1]) - 2);
      (void) memset(querystr, 0, sizeof(querystr));
      (void) snprintf(querystr,
		      sizeof(querystr),
		      "SELECT IFNULL(MAX(ABS(id)), 1) + 1 FROM coin WHERE "
		      "user_name = '%s'", user_name);

      if(mysql_query(connection, querystr) != 0)
	{
	  error = 1;
	  goto error;
	}

      res_set = mysql_store_result(connection);

      if(res_set == NULL)
	{
	  error = 1;
	  goto error;
	}

      row = mysql_fetch_row(res_set);

      if(row == NULL)
	{
	  error = 1;
	  goto error;
	}

      id = row[0];
      (void) memset(querystr, 0, sizeof(querystr));
      (void) memset(indata, 0, sizeof(indata));

      if(fgets(indata, (int) sizeof(indata) - 1, stdin) == NULL)
	{
	  error = 1;
	  goto error;
	}

      tmp = strtok(indata, "&");
      (void) strncpy(querystr, "INSERT INTO coin VALUES (",
		     sizeof(querystr) - 1);

      while(tmp != NULL)
	{
	  if(i++ <= 6)
	    {
	      update(field, tmp, sizeof(field), ADD);
	    }
	  else
	    {
	      (void) strncpy(field, tmp, sizeof(field) - 1);
	    }

	  (void) strncat(querystr, field, sizeof(querystr) - 1);
	  (void) strncat(querystr, ", ", sizeof(querystr) - 1);
	  tmp = strtok(NULL, "&");
	}

      querystr[strlen(querystr) - 2] = '\0';
      (void) strncat(querystr, ",", sizeof(querystr) - 1);
      (void) strncat(querystr, id, sizeof(querystr) - 1);
      (void) strncat(querystr, ")", sizeof(querystr) - 1);
      i = 0;

      while(querystr[i] != '(' && i++ < strlen(querystr));

      (void) memset(querystr1, 0, sizeof(querystr1));
      (void) strncpy(querystr1, querystr, i + 1);
      k = 0;
      j = strlen(querystr1);

      while(k < sizeof(sizes) / sizeof(sizes[0]))
      {
	i += sizes[k++];

	while(querystr[i] != ',' && i < strlen(querystr))
	  {
	    querystr1[j++] = querystr[i++];
	  }
      }

      (void) strncat(querystr1, ",", sizeof(querystr1) - 1);
      (void) strncat(querystr1, id, sizeof(querystr1) - 1);
      (void) strncat(querystr1, ",'", sizeof(querystr1) - 1);
      (void) strncat(querystr1, user_name, sizeof(querystr1) - 1);
      (void) strncat(querystr1, "')", sizeof(querystr1) - 1);

      for(i = 0; i < strlen(querystr1); i++)
	{
	  if(querystr1[i] == '=')
	    {
	      querystr1[i] = ',';
	    }
	}

      error = 0;

      if(mysql_query(connection, querystr1) == 0)
	{
	  (void) printf("<center>%sInformation saved.%s<br><br>"
			"</center>\n", FBEG, FEND);
	  (void) printf("<script>"
			"if(this.window != null && "
			"   this.window.opener != null && "
			"   !this.window.opener.closed)"
			"this.window.opener.refresh();</script>\n");

	  /*
	  ** Redisplay.
	  */

	  (void) printf("<script>this.window.close();\n"
			"this.window.opener.add_ent();</script>");
	}
      else
	{
	  error = 1;
	}

    error:

      if(error == 1)
	{
	  (void) printf("<center>%sInsert error.<br>"
			"Please be sure to complete all required "
			"fields.%s<br><br></center>\n",
			FBEG, FEND);
	}

      mysql_close(connection);
    }

  (void) printf("<center>\n");
  (void) printf("<a href=\"javascript:history.go(-1)\">%sBack%s</a> | ",
		FBEG, FEND);
  (void) printf("<a href=\"javascript:window.close()\">%sClose%s</a>\n",
		FBEG, FEND);
  (void) printf("</center>\n");
  (void) printf("</body></html>");
  return EXIT_SUCCESS;
}
