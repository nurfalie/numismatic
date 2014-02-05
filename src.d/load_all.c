/*
** -- Standard Includes --
*/

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
  char indata[MAX_1];
  char query[MAX_2];
  char *tmp = 0;
  char user_name[33];
  double acq_total = 0.0;
  double mrk_total = 0.0;
  double total = 0.0;
  int ct = 0;
  long quantities = 0;
  size_t i = 0;

  /*
  ** Load the entries and display them in HTML-friendly format.
  */

  if((connection = mysql_init(0)) == 0)
    {
      (void) printf("Content-type: text/html\n\n");
      (void) printf("<html>\n");
      (void) printf("<title>Numismatic - Main</title>\n");
      (void) printf("<body bgcolor=\"white\">\n");
      (void) printf("<center>%sInitialization error.%s</center><br>\n",
		    FBEG, FEND);
      (void) printf("</body></html>\n");
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
      (void) printf("Content-type: text/html\n\n");
      (void) printf("<html>\n");
      (void) printf("<title>Numismatic - Main</title>\n");
      (void) printf("<body bgcolor=\"white\">\n");
      (void) printf("<center>%sConnection error.%s</center><br>\n",
		    FBEG, FEND);
      (void) printf("</body></html>\n");
      return EXIT_FAILURE;
    }

  (void) printf("Content-type: text/html\n\n");

  if((tmp = getenv("REMOTE_USER")) == 0)
    tmp = "unknown";

  (void) memset(user_name, '\0', sizeof(user_name));
  (void) snprintf(user_name, sizeof(user_name), "%s", tmp);
  (void) memset(indata, 0, sizeof(indata));

  if(argc > 1)
    {
      (void) strncpy(indata, argv[1], sizeof(indata) - 1);
    }
  else
    {
      (void) strncpy(indata, "name", sizeof(indata) - 1);
    }

  /*
  ** Load and display the rows.
  */

  (void) memset(query, 0, sizeof(query));
  (void) snprintf(query,
		  sizeof(query),
		  "SELECT name, country, grade, "
		  "composition, weight, year, face_val, "
		  "acq_val, market_val, quantity, "
		  "quantity * market_val total, "
		  "IF0(description, ''), id FROM coin "
		  "WHERE id > 0 AND user_name = '%s' ORDER BY %s",
		  user_name, indata);

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

	  (void) printf("<table align=\"center\">\n");
	  (void) printf("<tr><td>\n");
	  (void) printf("%s<a href=\"javascript:check_boxes()\">"
			"Check All</a> | "
			"<a href=\"javascript:clear_boxes()\""
			">Clear All</a> | "
			"<a href=\"javascript:delete_ent()\">"
			"Delete Selected</a> | "
			"<a href=\"javascript:add_ent('%s')\">Add Item</a> | "
			"<a href=\"javascript:print_ent('%s')\">"
			"View Print-Friendly Format</a> | "
			"<a href=\"javascript:window.close()\">Close</a>"
			"%s<br><hr>\n",
			FBEG, user_name, user_name, FEND);
	  (void) printf("<table align=\"center\" border=1 "
			"cellspacing=0 cellpadding=5>\n");
	  (void) printf("<tr bgcolor=\"#EEEEEE\">\n");
	  (void) printf("<th>%sID%s</th>\n", FBEG, FEND);
	  (void) printf("<th>&nbsp</th>\n");
	  (void) printf("<th><a href=\"%s\">%sName%s</a></th>\n",
			"javascript:reloadall(1)",
			FBEG, FEND);
	  (void) printf("<th><a href=\"%s\">%sCountry%s</a></th>\n",
			"javascript:reloadall(2)",
			FBEG, FEND);
	  (void) printf("<th><a href=\"%s\">%sGrade%s</a></th>\n",
			"javascript:reloadall(3)",
			FBEG, FEND);
	  (void) printf("<th><a href=\"%s\">%sComposition%s</a></th>\n",
			"javascript:reloadall(4)",
			FBEG, FEND);
	  (void) printf("<th>%sWeight%s</th>\n", FBEG, FEND);
	  (void) printf("<th><a href=\"%s\">%sYear%s</a></th>\n",
			"javascript:reloadall(5)",
			FBEG, FEND);
	  (void) printf("<th>%sFace Value%s</th>\n", FBEG, FEND);
	  (void) printf("<th><a href=\"%s\">%sAcquisition Value ($)"
			"%s</a></th>\n",
			"javascript:reloadall(6)",
			FBEG, FEND);
	  (void) printf("<th><a href=\"%s\">%sMarket Value ($)%s</a></th>\n",
			"javascript:reloadall(7)",
			FBEG, FEND);
	  (void) printf("<th><a href=\"%s\">%sQuantity%s</a></th>\n",
			"javascript:reloadall(8)",
			FBEG, FEND);
	  (void) printf("<th>%sTotal ($)%s</th>\n", FBEG, FEND);
	  (void) printf("<th>%sDescription%s</th>\n", FBEG, FEND);
	  (void) printf("<th>&nbsp</th>\n");
	  (void) printf("</tr>\n");
	  (void) printf("<form name=\"mainform\">\n");

	  while((row = mysql_fetch_row(res_set)) != 0)
	    {
	      ct += 1;
	      (void) printf("<tr bgcolor=\"beige\" align=\"center\">\n");
	      (void) printf("<td>%s%d%s</td>\n", FBEG, ct, FEND);
	      (void) printf("<td><input type=\"checkbox\" name=\"cblist\" "
			    "value=\"%s\"></td>\n",
			    row[mysql_num_fields(res_set) - 1]);

	      /*
	      ** mysql_num_fields(res_set) - 1 => skip the ID field.
	      */

	      for(i = 0; i < mysql_num_fields(res_set) - 1; i++)
		{
		  (void) printf("<td>%s%s%s%s</td>\n", FBEG,
				(i == 7 || i == 8 || i == 10) ? "$" : "",
				(strlen(row[i]) == 0) ? "&nbsp" : row[i],
				FEND);

		  if(i == 7)
		    acq_total += atof(row[i]);
		  else if(i == 8)
		    mrk_total += atof(row[i]);
		  else if(i == 9)
		    quantities += atol(row[i]);
		  else if(i == 10)
		    total += atof(row[i]);
		}

	      (void) printf("<td><a href=\"javascript:open_edit(%s, "
			    "'%s')\">"
			    "%sEdit%s</a></td>\n", row[i], user_name,
			    FBEG, FEND);
	      (void) printf("</tr>");
	    }

	  (void) printf("</form>\n");
	  (void) printf("<tr bgcolor=\"#EEEEEE\" align=\"center\">\n");
	  (void) printf("<th>Totals</th>\n");
	  (void) printf("<th>&nbsp</th>\n");
	  (void) printf("<th>&nbsp</th>\n");
	  (void) printf("<th>&nbsp</th>\n");
	  (void) printf("<th>&nbsp</th>\n");
	  (void) printf("<th>&nbsp</th>\n");
	  (void) printf("<th>&nbsp</th>\n");
	  (void) printf("<th>&nbsp</th>\n");
	  (void) printf("<th>&nbsp</th>\n");
	  (void) printf("<th>%s$%.2f%s</th>\n", FBEG, acq_total, FEND);
	  (void) printf("<th>%s$%.2f%s</th>\n", FBEG, mrk_total, FEND);
	  (void) printf("<th>%s%ld%s</th>\n", FBEG, quantities, FEND);
	  (void) printf("<th>%s$%.2f%s</th>\n", FBEG, total, FEND);
	  (void) printf("<th>&nbsp</th>\n");
	  (void) printf("<th>&nbsp</th>\n");
	  (void) printf("</tr>\n");
	  (void) printf("</table>\n");
	  (void) printf("</td></tr>\n<tr><td>\n");
	  (void) printf("<hr>%s<a href=\"javascript:check_boxes()\">"
			"Check All</a> | "
			"<a href=\"javascript:clear_boxes()"
			"\">Clear All</a> | "
			"<a href=\"javascript:delete_ent()\">"
			"Delete Selected</a> | "
			"<a href=\"javascript:add_ent('%s')\">Add Item</a> | "
			"<a href=\"javascript:print_ent('%s')\">"
			"View Print-Friendly Format</a> | "
			"<a href=\"javascript:window.close()\">Close</a>"
			"%s</td></tr>\n",
			FBEG, user_name, user_name, FEND);
	  (void) printf("</table>\n");

	  /*
	  ** Free some space.
	  */

	  mysql_free_result(res_set);
	}

      (void) printf("<br>");
    }
  else
    {
      (void) printf("<center>%sFetch error.%s</center><br>\n", FBEG, FEND);
      mysql_close(connection);
      return EXIT_FAILURE;
    }

  mysql_close(connection);
  return EXIT_SUCCESS;
}
