<%@ Page Language="C#" %>
 
 <html>
 
<head>
 
    <title>Shibboleth Echo Page</title>
 
</head>
 
<body>
 
    You are logged in using Shibboleth!
 
    <hr />
 
    <table>
 
        <%
 
foreach( string key in Request.ServerVariables )
 
{
 
    if( key.StartsWith("HTTP_SHIB" ) )
 
    {
 
        %>
 
        <tr>
 
            <td>
 
                <%= key %>
 
            </td>
 
            <td>
 
                <%= Request.ServerVariables[ key ] %>
 
            </td>
 
        </tr>
 
        <%
 
    }
 
}
 
        %>
 
    </table>
 
    <hr />
 
    <a href="http://blogs.msdn.com/Shibboleth.sso/Logout">Logout</a>
 
</body>
 
</html> 
