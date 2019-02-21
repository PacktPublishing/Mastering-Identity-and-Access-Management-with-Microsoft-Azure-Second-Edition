<%@ Page Title="Home Page" Language="C#" MasterPageFile="~/Site.master" AutoEventWireup="true"
    CodeFile="Default.aspx.cs" Inherits="_Default" ValidateRequest="false" %>

<%@ Register assembly="SecurityTokenVisualizerControl" namespace="Microsoft.Samples.DPE.Identity.Controls" tagprefix="cc1" %>


<asp:Content ID="HeaderContent" runat="server" ContentPlaceHolderID="HeadContent">
</asp:Content>
<asp:Content ID="BodyContent" runat="server" ContentPlaceHolderID="MainContent">
    <h2>
        Welcome ClaimsWeb!





    </h2>
    <br />
    <br />
    

    <cc1:SecurityTokenVisualizerControl ID="SecurityTokenVisualizerControl1"  runat="server" />

</asp:Content>
