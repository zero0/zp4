<%@ include file="include.jsp" %>

<%
SingularityEngine engine = SingularityEngine.getInstance();
String id = request.getParameter("id");
SingularityProject project = engine.getProject( id );
String name = project.getName();
List< String > taskNames = project.getTaskNames();
SingularityEnvironment env = project.getEnvironment();
List< String > envVariables = env.getVariableKeys();
%>

<s:master-page page="template.jsp">
  <s:content name="sub-title">- Project: <%=name%></s:content>

  <s:content name="body-content">
    <div>
      <div class="page-header">
        <h1><%=name%></h1>
      </div>
      <div class="container">
        <h2>Tasks</h2>
        <table class="table table-striped">
          <colgroup>
          </colgroup>
          <thead>
            <tr>
              <th>Task</th>
              <th>Execute</th>
              <th>Last Execution</th>
            </tr>
          </thead>
          <tfoot></tfoot>
          <tbody>
            <% for( int i = 0, imax = taskNames.size(); i < imax; ++i ) { String taskName = taskNames.get( i ); %>
            <tr>
              <td><%=taskName%></td>
              <td>Exectute</td>
              <td>Success</td>
            </tr>
            <% } %>
          </tbody>
        </table>
      </div>
      <div class="container">
        <h2>Environment Variables</h2>
        <table class="table table-striped table-hover">
          <colgroup>
          </colgroup>
          <thead>
            <tr>
              <th>Key</th>
              <th>Value</th>
              <th>Raw</th>
            </tr>
          </thead>
          <tfoot></tfoot>
          <tbody>
            <% for( int i = 0, imax = envVariables.size(); i < imax; ++i ) { String envVar = envVariables.get( i ); %>
            <tr>
              <td><%=envVar%></td>
              <td><%=env.getVariable( envVar )%></td>
              <td><%=env.getVariableRaw( envVar )%></td>
            </tr>
            <% } %>
          </tbody>
        </table>
      </div>
    </div>
  </s:content>
</s:master-page>
