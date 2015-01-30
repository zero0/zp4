<%@ page import="java.util.*,java.text.*,org.zero0.zeropoint.tools.arc.*" %>
<%
Arc arc = Arc.getInstance();
DateFormat df = new SimpleDateFormat("MM/dd/yyyy hh:mm:ss aa");
%>
<html>
	<head>
		<title>ARC</title>
		<link href="theme/jquery-ui.css" rel="stylesheet">
		<style>
			body
			{
				padding: 0;
				margin: 0;
				font-size: .6em;
			}
			
			#info
			{
				position: absolute;
				left: 10;
				top: 10;
			}
		</style>
		<script type="text/javascript" src="http://code.jquery.com/jquery-2.1.3.min.js"></script>
		<script type="text/javascript" src="theme/jquery-ui.js"></script>
		<script type="text/javascript">
			$(function()
			{
				$( "#tabs" ).tabs();
				
				$( "#refresh" ).button({
					icons: {
						primary: 'ui-icon-refresh'
					}
				});
				
				$( "#rescan" ).button({
					icons: {
						primary: 'ui-icon-document'
					}
				});
				
				$( "button[id^='compile-']" ).button({
					text: false,
					icons: {
						primary: 'ui-icon-gear'
					}
				}).click(function() {
					var url = "arc?" + $.param( { compile: $(this).data( "local-file-name" ) } );
					
					$.ajax({
						dataType: "text",
						type: "GET",
						url: url,
						cache: false,
						crossDomain: true,
						isLocal: true,
						xhrFields: {
							withCredentials: true
						}
					}).done(function( data ) {
						$( "#info" ).show( "Drop", {}, 500, function() {
							$( "#info div" ).text( "Compiled" );
							
							setTimeout(function() {
								$( "#info" ).fadeOut();
							}, 2000 );
						});
					}).fail(function( jqXHR, textStatus ) {
						alert( "Failed " + textStatus );
					});
				});
				
				$( "button[id^='view-']" ).button({
					text: false,
					icons: {
						primary: 'ui-icon-search'
					}
				}).click( function() {
					$( "#view-file-dialog" ).data( "file-name", $(this).data( "local-file-name" ) );
					$( "#view-file-dialog" ).dialog( "option", "title", $(this).data( "local-file-name" ) );
					$( "#view-file-dialog" ).dialog( "open" );
				}).parent().buttonset();
				
				$( "#view-file-dialog" ).dialog({
					autoOpen: false,
					height: 540,
					width: 360,
					position: [100, 100],
					modal: false,
					buttons: {
						'Close': function () {
							$(this).dialog('close');
						}
					},
					close: function () {
						$( "#view-file-contents" ).text( "" );
					},
					open: function() {
						var url = "arc?" + $.param( { file: $( "#view-file-dialog" ).data( "file-name" ) } );
						
						$.ajax({
							dataType: "text",
							type: "GET",
							url: url,
							cache: false,
							crossDomain: true,
							isLocal: true,
							xhrFields: {
								withCredentials: true
							}
						}).done(function( data ) {
							$( "#view-file-contents" ).text( data );
						}).fail(function( jqXHR, textStatus ) {
							alert( "Failed " + textStatus );
						});
					}
				});
				
				$("#auto-compile").button({
					icons: {
						primary: 'ui-icon-file'
					}
				}).click(function() {
					var isChecked = $(this).is(":checked");
					if( isChecked )
					{
						<% arc.setAutoCompile( true ); %>
					}
					else
					{
						<% arc.setAutoCompile( false ); %>
					}
				});
			});
		</script>
	</head>
	<body>
		<div id="tabs">
			<ul>
				<li><a href="#tabs-1">Files</a></li>
				<li><a href="#tabs-2">Options</a></li>
				<li><a href="#tabs-3">View</a></li>
			</ul>
			<div id="tabs-1">
				<button id="refresh">Refresh</button>
				<button id="rescan">Rescan</button>
			</div>
			<div id="tabs-2">
				<input type="checkbox" id="auto-compile"/><label for="auto-compile">Auto-Compile</label>
				<span>
					<input type="checkbox" id="rendering-dx11"/><label for="rendering-dx11">DX11</label>
					<input type="checkbox" id="rendering-dx10"/><label for="rendering-dx10">DX10</label>
					<input type="checkbox" id="rendering-gl"/><label for="rendering-gl">GL</label>
				</span>
			</div>
			<div id="tabs-3"></div>
		</div>

		<div class="ui-widget">
			<table class="ui-widget-content" width="100%">
				<thead class="ui-widget-header">
					<tr>
						<th></th>
						<th>File</th>
						<th>Last Modified</th>
					</tr>
				</thead>
				<tfoot>
				</tfoot>
				<tbody class="ui-widget-content">
				<%
					List<String> allFiles = new ArrayList<String>( arc.getFiles() );
					Collections.sort( allFiles );
					int i = -1;
					for( String file : allFiles )
					{
						++i;
						String localFile = file.replace( arc.getAssetsDirectory(), "" );
				%>
					<tr>
						<td>
							<span>
								<button id="compile-<%=i%>" data-file-name="<%=file%>" data-local-file-name="<%=localFile%>"></button>
								<button id="view-<%=i%>" data-file-name="<%=file%>" data-local-file-name="<%=localFile%>"></button>
							</span>
						</td>
						<td><%=localFile%></td>
						<td><%=df.format( new Date( arc.getFileModificationTime( file ) ) )%></td>
					</tr>
				<% } %>
				</tbody>
			</table>
		</div>
		
		<div id="view-file-dialog">
			<div>
				<pre id="view-file-contents"></pre>
			</div>
		</div>
		<div id="info" class="ui-widget">
			<div class="ui-state-highlight ui-corner-all">
			
			</div>
		</div>
	</body>
</html>
