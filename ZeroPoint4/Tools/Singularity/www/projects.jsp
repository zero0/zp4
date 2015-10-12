<%@ include file="include.jsp" %>

<% SingularityEngine engine = SingularityEngine.getInstance(); %>
<s:master-page page="template.jsp">
  <s:content name="sub-title">- Projects</s:content>

  <s:content name="body-content">
    <div>
      <div class="page-header">
        <h1>Projects</h1>
      </div>
      <div class="container">
        <table class="table table-striped">
          <colgroup>
            <col width="50%">
            <col width="50%">
          </colgroup>
          <thead>
            <tr>
              <th>Name</th>
              <th>Status</th>
            </tr>
          </thead>
          <tfoot></tfoot>
          <tbody>
            <% for( int i = 0, imax = engine.getNumProjects(); i < imax; ++i ) { SingularityProject project = engine.getProject( i ); %>
            <tr>
              <td><a href="project.jsp?id=<%=project.getId()%>"><%=project.getName()%></a></td>
              <td></td>
            </tr>
            <% } %>
          </tbody>
        </table>
      </div>
    </div>
  </s:content>
</s:master-page>
