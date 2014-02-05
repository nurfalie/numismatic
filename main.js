function statusMsg(msg)
{
    window.status=msg;
    return true;
}

function reloadall(bywhat)
{
    if(this != null && this.window != null && this.window.opener != null &&
       this.window.opener.window != null)
	this.window.opener.window.launch_app(bywhat);
}

function winclose()
{
    this.close();
}

function add_ent(userid)
{
    var add_w1 = window.open
    ("/cgi-bin/numismatic/add.cgi?" + userid,
     "Add", "resizable,width=675,height=625,scrollbars=yes");
}

function open_edit(id, userid)
{
    var edit_w1 = window.open
    ("/cgi-bin/numismatic/edit.cgi?" + id + "?" + userid, "Edit",
     "resizable,width=675,height=625,scrollbars=yes");
}

function print_ent(userid)
{
    var print_w1 = window.open
    ("/cgi-bin/numismatic/print.cgi?" + userid,
     "Print", "resizable,width=800,height=680,scrollbars=yes,toolbar=yes");
}

function delete_ent()
{
    var info = "/cgi-bin/numismatic/delete.cgi?";

    if(document.mainform.cblist && document.mainform.cblist.length)
	for(i = 0; i < document.mainform.cblist.length; i++)
    {
	if(document.mainform.cblist[i].checked)
	    info = info + document.mainform.cblist[i].value + ",";
    }
    else if(document.mainform.cblist)
    {
	if(document.mainform.cblist.checked)
	    info = info + document.mainform.cblist.value;
    }

    if(info != "/cgi-bin/numismatic/delete.cgi?")
    {
	if(confirm("Are you sure you wish to delete the selected item(s)?"))
	{
	    var info_w1 = window.open
	    (info, "Delete",
	     "resizable,width=350,height=350,scrollbars=yes");

	    clear_boxes();
	}
    }
}

function refresh()
{
    if(this != null && this.window != null && this.window.location != null)
	this.window.location.reload();
}

function check_boxes()
{
    if(document.mainform.cblist && document.mainform.cblist.length)
	for(i = 0; i < document.mainform.cblist.length; i++)
	    document.mainform.cblist[i].checked = true;
    else if(document.mainform.cblist)
	document.mainform.cblist.checked = true;
}

function clear_boxes()
{
    if(document.mainform.cblist && document.mainform.cblist.length)
	for(i = 0; i < document.mainform.cblist.length; i++)
	    document.mainform.cblist[i].checked = false;
    else if(document.mainform.cblist)
	document.mainform.cblist.checked = false;
}
