using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using Microsoft.IdentityModel.Claims;
using System.Threading;

public partial class _Default : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {

        if (Page.User.Identity.IsAuthenticated)
        {
            IClaimsIdentity claimsIdentity = Thread.CurrentPrincipal.Identity as IClaimsIdentity;
            //ShowName(claimsIdentity);
            //ShowClaimsIdentityAsIIdentity(claimsIdentity);
            //ShowClaimsFromClaimsIdentity(claimsIdentity);



        }
    }
}
