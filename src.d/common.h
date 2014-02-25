/*
** Copyright (c) 2003 - Eternity Alexis Megas.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _COMMON_H_
#define _COMMON_H_

/*
** -- C Includes --
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
** -- Definitions --
*/

#define MYDB "xnumismatic_db"
#define MYHOST "localhost"
#define MYPASSWORD "xnumismatic_user"
#define MYUSER "xnumismatic_user"

#define FBEG "<font size=2>"
#define FEND "</font>"

#define MAX_1 1024
#define MAX_2 2048
#define MAX_QUAN 500
#define YEAR_END 2015
#define YEAR_START 1400

const char *COUNTRIES[] = {"N/A",
			   "Australia",
			   "Austria",
			   "Canada",
			   "China",
			   "France",
			   "Germany",
			   "Greece",
			   "Ireland",
			   "Italy",
			   "Mexico",
			   "Netherlands",
			   "Romania",
			   "Russia",
			   "Scotland",
			   "Spain",
			   "United Kingdom",
			   "United States Of America",
			   "Wales"};
const char *GRADES[] = {"N/A",
			"AU",
			"BU",
			"Extra Fine",
			"Fine",
			"Mint",
			"Poor",
			"Proof",
			"Very Fine"};
const short ADD = 0;
const short EDIT = 1;

/*
** -- Function Declarations --
*/

void update(char *, char *, const size_t, const short);

/*
** -- Function Definitions --
*/

void update(char *field, char *tmp,
	    const size_t field_size, const short type)
{
  char hex[5];
  char *stop;
  char tmpstr[3];
  char tmpstr1[MAX_1];
  size_t c = 0;
  size_t i = 0;

  if(!field || !tmp)
    return;

  hex[0] = '0';
  hex[1] = 'x';
  hex[4] = '\0';
  (void) memset(field, 0, field_size);
  (void) memset(tmpstr, 0, sizeof(tmpstr));
  (void) memset(tmpstr1, 0, sizeof(tmpstr1));

  for(i = 0; i < strlen(tmp); i++)
    {
      if(tmp[i] == '%')
	{
	  (void) memset(tmpstr, 0, sizeof(tmpstr));

	  if(i + 1 < strlen(tmp))
	    hex[2] = tmp[i + 1];

	  if(i + 2 < strlen(tmp))
	    hex[3] = tmp[i + 2];

	  if(i + 1 < strlen(tmp) && i + 2 < strlen(tmp) &&
	     tmp[i + 1] == '2' && tmp[i + 2] == '7')
	    {
	      tmpstr[0] = '\\';
	      tmpstr[1] = '\'';
	    }
	  else
	    tmpstr[0] = (char) strtol(hex, &stop, 16);

	  if(sizeof(tmpstr1) > strlen(tmpstr1))
	    (void) strncat
	      (tmpstr1, tmpstr, sizeof(tmpstr1) - strlen(tmpstr1) - 1);

	  i += 2;
	}
      else if(tmp[i] == '+')
	{
	  if(sizeof(tmpstr1) > strlen(tmpstr1))
	    (void) strncat(tmpstr1, " ",
			   sizeof(tmpstr1) - strlen(tmpstr1) - 1);
	}
      else if(tmp[i] == '=')
	{
	  (void) memset(tmpstr, 0, sizeof(tmpstr));
	  tmpstr[0] = '\'';

	  if(sizeof(tmpstr1) > strlen(tmpstr1))
	    (void) strncat(tmpstr1, "=",
			   sizeof(tmpstr1) - strlen(tmpstr1) - 1);

	  if(sizeof(tmpstr1) > strlen(tmpstr1))
	    (void) strncat
	      (tmpstr1, tmpstr, sizeof(tmpstr1) - strlen(tmpstr1) - 1);
	}
      else if(isalnum(tmp[i]) || isblank(tmp[i]) || tmp[i] == '.' ||
	      tmp[i] == '-')
	{
	  (void) memset(tmpstr, 0, sizeof(tmpstr));
	  tmpstr[0] = tmp[i];

	  if(sizeof(tmpstr1) > strlen(tmpstr1))
	    (void) strncat
	      (tmpstr1, tmpstr, sizeof(tmpstr1) - strlen(tmpstr1) - 1);
	}
    }

  if(sizeof(tmpstr1) > strlen(tmpstr1))
    (void) strncat(tmpstr1, "'", sizeof(tmpstr1) - strlen(tmpstr1) - 1);

  for(i = 0; i < strlen(tmpstr1) - 1; i++)
    {
      if(tmpstr1[i] == '\'' && tmpstr1[i + 1] == '\'')
	{
	  c = i++;

	  /*
	  ** Most of the fields are required.
	  ** Use "NULL" if a field can be set to NULL.
	  */

	  if(type == ADD)
	    {
	      if(field_size > strlen(field))
		(void) strncat(field, "NULL", field_size - strlen(field) - 1);
	    }
	  else if(type == EDIT)
	    {
	      if(field_size > strlen(field))
		(void) strncat(field, "", field_size - strlen(field) - 1);
	    }
	}
      else
	{
	  tmpstr[0] = tmpstr1[i];

	  if(field_size > strlen(field))
	    (void) strncat(field, tmpstr, field_size - strlen(field) - 1);
	}
    }

  if(c == 0)
    {
      if(field_size > strlen(field))
	(void) strncat(field, "'", field_size - strlen(field) - 1);
    }

  if(strcmp(field, "description=") == 0)
    {
      (void) memset(field, 0, field_size);
      (void) snprintf(field, field_size, "%s", "description=NULL");
    }
}

#endif
