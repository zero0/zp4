package org.zero0.zeropoint.tools.arc.workspace;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.regex.Pattern;
import java.util.zip.ZipFile;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.SashForm;
import org.eclipse.swt.custom.StyleRange;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.dnd.DND;
import org.eclipse.swt.dnd.DropTarget;
import org.eclipse.swt.dnd.DropTargetAdapter;
import org.eclipse.swt.dnd.DropTargetEvent;
import org.eclipse.swt.dnd.FileTransfer;
import org.eclipse.swt.dnd.Transfer;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.Device;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.ProgressBar;
import org.eclipse.swt.widgets.TabFolder;
import org.eclipse.swt.widgets.TabItem;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.TableItem;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.ToolBar;
import org.eclipse.swt.widgets.ToolItem;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeColumn;
import org.eclipse.swt.widgets.TreeItem;
import org.zero0.zeropoint.tools.arc.Arc;
import org.zero0.zeropoint.tools.arc.ArcListener;
import org.zero0.zeropoint.tools.arc.ExecutableMode;
import org.zero0.zeropoint.tools.arc.OutputLevel;
import org.zero0.zeropoint.tools.arc.Platform;
import org.zero0.zeropoint.tools.arc.Rendering;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompiler;
import org.zero0.zeropoint.tools.arc.util.FileWatcherAdapter;
import org.zero0.zeropoint.tools.arc.util.FileWatcherListener;
import org.zero0.zeropoint.tools.arc.util.OutputAppender;
import org.zero0.zeropoint.tools.arc.util.PrintErrAppender;
import org.zero0.zeropoint.tools.arc.util.PrintErrWrapper;
import org.zero0.zeropoint.tools.arc.util.PrintOutAppender;
import org.zero0.zeropoint.tools.arc.util.PrintOutWrapper;

public class Workspace extends Composite implements PrintOutAppender, PrintErrAppender, OutputAppender, FileWatcherListener, ArcListener
{
	static Map<String, Image> cachedImages = new HashMap<String, Image>();
	static Map<Integer, Color> cachedColors = new HashMap<Integer, Color>();
	
	private static Image CreateImage( Device device, String imageFile )
	{
		Image img = null;

		if( cachedImages.containsKey( imageFile ) )
		{
			img = cachedImages.get( imageFile );
		}
		else
		{
			int sep = imageFile.indexOf( ":" );
			if( sep > -1 )
			{
				try
				{
					String archive = imageFile.substring( 0, sep );
					String image = imageFile.substring( sep + 1 );

					ZipFile zip = new ZipFile( new File( archive ) );
					img = new Image( device, new BufferedInputStream( zip.getInputStream( zip.getEntry( image ) ) ) );
				}
				catch( Exception e )
				{
					e.printStackTrace();
				}
			}
			else
			{
				img = new Image( device, imageFile );
			}
			cachedImages.put( imageFile, img );
		}
		return img;
	}

	private static Image CreateIcon( Device device, String icon )
	{
		return CreateImage( device, "res/famfamfam_silk_icons_v013.zip:icons/" + icon + ".png" );
	}
	
	private static Color CreateColor( Device device, int r, int g, int b )
	{
		Color c = null;
		Integer hash = ( r & 0xFF ) << 16 | ( g & 0xFF ) << 8 | ( b & 0xFF );
		
		if( cachedColors.containsKey( hash ) )
		{
			c = cachedColors.get( hash );
		}
		else
		{
			c = new Color( device, r, g, b );
			cachedColors.put( hash, c );
		}
		
		return c;
	}

	static final FormAttachment fa0 = new FormAttachment( 0 );
	static final FormAttachment fa100 = new FormAttachment( 100 );

	TabFolder tabs;
	TabItem mainTab;
	TabItem dbTab;

	SashForm form;

	Text search;
	ToolBar bar;
	Tree tree;

	TabFolder textTabs;
	StyledText console;
	StyledText fileContent;

	Composite top;
	Composite middle;
	Composite bottom;

	Composite comboComposite;
	Composite treeComposite;
	Composite textComposite;

	Menu menu;

	DropTarget dropTarget;

	Label statusLabel;
	Label optionsLabel;
	Combo outputCombo;
	Combo platformCombo;
	Combo renderingCombo;

	Text dbSearch;
	Button dbRun;
	Table dbResults;
	List<String[]> dbResultsData;

	ProgressBar progress;

	PrintOutWrapper printOut;
	PrintErrWrapper printErr;

	String treeFilter;

	Properties properties;
	
	boolean showAllFilesInRoot = false;

	private static final String workspacePropertyFile = "workspace.properties";
	private final int progressMax = 100;
	
	public Workspace( Composite parent )
	{
		super( parent, SWT.NONE );

		properties = new Properties();
		try
		{
			properties.load( new BufferedInputStream( new FileInputStream( workspacePropertyFile ) ) );
		}
		catch( Exception e )
		{
			e.printStackTrace();
		}

		getShell().setImage( CreateIcon( getDisplay(), properties.getProperty( "arc.workspace.app.icon" ) ) );
		setLayout( new FillLayout() );

		tabs = new TabFolder( this, SWT.NONE );

		createMainTab();

		createDbTab();

		Arc.getInstance().addFileWatcherListener( this );
		printOut = new PrintOutWrapper( this );
		printErr = new PrintErrWrapper( this );
		System.setOut( printOut );
		System.setErr( printErr );
		//OutputStream out = new OutputStream() {
		//@Override
		//public void write( int b ) throws IOException {
		//text.append( Character.toString( (char)b ) );
		//}
		//};
		//System.setOut( new PrintStream( out ) );
		//System.setErr( new PrintStream( out ) );
	}

	void createMainTab()
	{
		mainTab = new TabItem( tabs, SWT.NONE );
		mainTab.setText( "Main" );

		Composite t = new Composite( tabs, SWT.NONE );
		mainTab.setControl( t );

		t.setLayout( new FormLayout() );

		top = new Composite( t, SWT.NONE );
		top.setLayout( new FillLayout() );

		middle = new Composite( t, SWT.NONE );
		middle.setLayout( new FillLayout() );

		bottom = new Composite( t, SWT.NONE );
		bottom.setLayout( new FillLayout() );

		FormData fd;

		fd = new FormData();
		fd.top = fa0;
		fd.left = fa0;
		fd.right = fa100;
		top.setLayoutData( fd );

		fd = new FormData();
		fd.top = new FormAttachment( top );
		fd.left = fa0;
		fd.right = fa100;
		fd.bottom = new FormAttachment( bottom );
		middle.setLayoutData( fd );

		fd = new FormData();
		fd.left = fa0;
		fd.right = fa100;
		fd.bottom = fa100;
		bottom.setLayoutData( fd );

		// createTop();

		createSash();

		createText();

		createTree();

		createMenu();

		createDropTarget();

		createBottom();

		updateOptionsText();
	}

	void createDbTab()
	{
		dbTab = new TabItem( tabs, SWT.NONE );
		dbTab.setText( "Database" );

		Composite t = new Composite( tabs, SWT.NONE );
		dbTab.setControl( t );

		t.setLayout( new FormLayout() );

		Composite dbtop = new Composite( t, SWT.NONE );
		dbtop.setLayout( new FormLayout() );

		dbSearch = new Text( dbtop, SWT.BORDER | SWT.SINGLE );

		dbRun = new Button( dbtop, SWT.PUSH );
		dbRun.setText( "Run" );
		dbRun.addSelectionListener( new SelectionListener()
		{
			@Override
			public void widgetSelected( SelectionEvent e )
			{
				widgetDefaultSelected( e );
			}

			@Override
			public void widgetDefaultSelected( SelectionEvent e )
			{
				String query = dbSearch.getText();
				if( !query.isEmpty() )
				{
					dbResultsData = Arc.getInstance().getArcDatabase().execQuery( query );
					if( !dbResultsData.isEmpty() )
					{
						dbResults.setRedraw( false );
						while( dbResults.getColumnCount() > 0 )
						{
							dbResults.getColumn( 0 ).dispose();
						}

						String[] columns = dbResultsData.remove( 0 );
						for( int i = 0; i < columns.length; ++i )
						{
							TableColumn column = new TableColumn( dbResults, SWT.NONE, i );
							column.setText( columns[i] );
							column.setWidth( 150 );
						}
						dbResults.setRedraw( true );
					}

					dbResults.setItemCount( Math.max( dbResultsData.size() - 1, 0 ) );
					dbResults.clearAll();
				}
			}
		} );

		Composite dbmiddle = new Composite( t, SWT.NONE );
		dbmiddle.setLayout( new FillLayout() );

		dbResults = new Table( dbmiddle, SWT.MULTI | SWT.FULL_SELECTION | SWT.BORDER | SWT.VIRTUAL );
		dbResults.setLinesVisible( true );
		dbResults.setHeaderVisible( true );
		dbResults.addListener( SWT.SetData, new Listener()
		{
			@Override
			public void handleEvent( Event event )
			{
				TableItem item = (TableItem) event.item;
				int index = dbResults.indexOf( item );
				if( index > -1 )
				{
					item.setText( dbResultsData.get( index ) );

					// if( index >= dbResultsData.size() - 2 )
					// {
					// for( int i = 0; i < dbResults.getColumnCount(); ++i )
					// {
					// dbResults.getColumn( i ).pack();
					// }
					// }
				}
			}
		} );

		Composite dbbottom = new Composite( t, SWT.NONE );
		dbbottom.setLayout( new FillLayout() );

		FormData fd;

		fd = new FormData();
		fd.top = fa0;
		fd.left = fa0;
		fd.right = new FormAttachment( dbRun );
		dbSearch.setLayoutData( fd );

		fd = new FormData();
		fd.top = fa0;
		fd.right = fa100;
		// fd.right = new FormAttachment( dbRun );
		dbRun.setLayoutData( fd );

		fd = new FormData();
		fd.top = fa0;
		fd.left = fa0;
		fd.right = fa100;
		dbtop.setLayoutData( fd );

		fd = new FormData();
		fd.top = new FormAttachment( dbtop );
		fd.left = fa0;
		fd.right = fa100;
		fd.bottom = new FormAttachment( dbbottom );
		dbmiddle.setLayoutData( fd );

		fd = new FormData();
		fd.left = fa0;
		fd.right = fa100;
		fd.bottom = fa100;
		dbbottom.setLayoutData( fd );
	}

	void createSash()
	{
		form = new SashForm( middle, SWT.HORIZONTAL );
		form.setLayout( new FillLayout() );

		treeComposite = new Composite( form, SWT.BORDER );
		treeComposite.setLayout( new FormLayout() );

		textComposite = new Composite( form, SWT.NONE );
		textComposite.setLayout( new FillLayout() );

		form.setWeights( new int[] { 60, 40 } );
	}

	void createTree()
	{
		TreeColumn col;

		search = new Text( treeComposite, SWT.BORDER | SWT.SINGLE );
		search.addListener( SWT.KeyUp, new Listener()
		{
			public void handleEvent( Event e )
			{
				filterTree( search.getText() );
			}
		} );

		bar = new ToolBar( treeComposite, SWT.NONE );

		ToolItem item;

		item = new ToolItem( bar, SWT.PUSH );
		item.setImage( CreateIcon( getDisplay(), "cancel" ) );
		item.addListener( SWT.Selection, new Listener()
		{
			@Override
			public void handleEvent( Event event )
			{
				search.setText( "" );
				filterTree( "" );
			}
		} );

		item = new ToolItem( bar, SWT.SEPARATOR );

		item = new ToolItem( bar, SWT.PUSH );
		item.setImage( CreateIcon( getDisplay(), "magnifier_zoom_in" ) );
		item.addListener( SWT.Selection, new Listener()
		{
			private void expand( TreeItem item )
			{
				item.setExpanded( true );
				for( int i = 0; i < item.getItemCount(); ++i )
				{
					expand( item.getItem( i ) );
				}
			}

			@Override
			public void handleEvent( Event event )
			{
				for( int i = 0; i < tree.getItemCount(); ++i )
				{
					expand( tree.getItem( i ) );
				}
			}
		} );

		item = new ToolItem( bar, SWT.PUSH );
		item.setImage( CreateIcon( getDisplay(), "magifier_zoom_out" ) );
		item.addListener( SWT.Selection, new Listener()
		{
			private void contract( TreeItem item )
			{
				item.setExpanded( false );
				for( int i = 0; i < item.getItemCount(); ++i )
				{
					contract( item.getItem( i ) );
				}
			}

			@Override
			public void handleEvent( Event event )
			{
				for( int i = 0; i < tree.getItemCount(); ++i )
				{
					contract( tree.getItem( i ) );
				}
			}
		} );

		item = new ToolItem( bar, SWT.PUSH );
		item.setImage( CreateIcon( getDisplay(), "arrow_refresh" ) );
		item.addListener( SWT.Selection, new Listener()
		{
			@Override
			public void handleEvent( Event event )
			{
				refreshTree();
			}
		} );
		
		item = new ToolItem( bar, SWT.SEPARATOR );
		
		item = new ToolItem( bar, SWT.PUSH );
		item.setImage( CreateIcon( getDisplay(), "wrench" ) );
		item.setToolTipText( "Build Tools" );
		item.addListener( SWT.Selection, new Listener()
		{
			@Override
			public void handleEvent( Event event )
			{
				Arc.getInstance().executeCompiler( "build-tools" );
			}
		} );
		
		item = new ToolItem( bar, SWT.PUSH );
		item.setImage( CreateIcon( getDisplay(), "wand" ) );
		item.setToolTipText( "Build Game" );
		item.addListener( SWT.Selection, new Listener()
		{
			@Override
			public void handleEvent( Event event )
			{
				Arc.getInstance().executeCompiler( "build-game" );
			}
		} );
		
		item = new ToolItem( bar, SWT.SEPARATOR );
		
		item = new ToolItem( bar, SWT.PUSH );
		item.setImage( CreateIcon( getDisplay(), "bin" ) );
		item.setToolTipText( "Clean Tools" );
		item.addListener( SWT.Selection, new Listener()
		{
			@Override
			public void handleEvent( Event event )
			{
				Arc.getInstance().executeCompiler( "clean-tools" );
			}
		} );
		
		item = new ToolItem( bar, SWT.PUSH );
		item.setImage( CreateIcon( getDisplay(), "bomb" ) );
		item.setToolTipText( "Clean Game" );
		item.addListener( SWT.Selection, new Listener()
		{
			@Override
			public void handleEvent( Event event )
			{
				Arc.getInstance().executeCompiler( "clean-game" );
			}
		} );
		
		item = new ToolItem( bar, SWT.SEPARATOR );
		
		item = new ToolItem( bar, SWT.PUSH );
		item.setImage( CreateIcon( getDisplay(), "joystick" ) );
		item.setToolTipText( "Run Game" );
		item.addListener( SWT.Selection, new Listener()
		{
			@Override
			public void handleEvent( Event event )
			{
				Arc.getInstance().executeCompiler( "run-game" );
			}
		} );
		
		
		tree = new Tree( treeComposite, SWT.VIRTUAL | SWT.FULL_SELECTION | SWT.MULTI );
		tree.setHeaderVisible( true );

		//Listener sortListener = new Listener()
		//{
		//	@Override
		//	public void handleEvent( Event e )
		//	{
		//		// determine new sort column and direction
		//		TreeColumn sortColumn = tree.getSortColumn();
		//		TreeColumn currentColumn = (TreeColumn) e.widget;
		//		int dir = tree.getSortDirection();
		//		if( sortColumn == currentColumn )
		//		{
		//			dir = dir == SWT.UP ? SWT.DOWN : SWT.UP;
		//		}
		//		else
		//		{
		//			tree.setSortColumn( currentColumn );
		//			dir = SWT.UP;
		//		}
		//
		//		int[] order = tree.getColumnOrder();
		//		int s = -1;
		//		for( int o : order )
		//		{
		//			if( currentColumn == tree.getColumn( order[o] ) )
		//			{
		//				s = order[o];
		//				break;
		//			}
		//		}
		//		
		//		TreeItem[] items = tree.getItems();
		//		
		//		// update data displayed in table
		//		tree.setSortDirection( dir );
		//	}
		//};
		
		col = new TreeColumn( tree, SWT.LEFT );
		col.setText( "File" );
		col.setWidth( 225 );
		col.setResizable( true );
		col.setMoveable( true );
		//col.addListener( SWT.Selection, sortListener );
		
		col = new TreeColumn( tree, SWT.LEFT );
		col.setText( "Type" );
		col.setWidth( 75 );
		col.setResizable( true );
		col.setMoveable( true );
		//col.addListener( SWT.Selection, sortListener );
		//tree.setSortColumn( col );
		//tree.setSortDirection( SWT.DOWN );
		
		col = new TreeColumn( tree, SWT.LEFT );
		col.setText( "Last Modified" );
		col.setWidth( 150 );
		col.setResizable( true );
		col.setMoveable( true );
		//col.addListener( SWT.Selection, sortListener );
		
		tree.addListener( SWT.SetData, new Listener()
		{
			Properties icons = Arc.getSubProperties( properties, "arc.workspace.ext.icon" );
			SimpleDateFormat dateFormat = new SimpleDateFormat( properties.getProperty( "arc.workspace.dateformat" ) );

			String trimFront( String str, String trim )
			{
				if( str.startsWith( trim ) )
				{
					return str.substring( trim.length() );
				}
				return str;
			}
			
			public void handleEvent( Event event )
			{
				List<String> allFiles = Arc.getInstance().getFiles();
				if( allFiles.isEmpty() )
				{
					return;
				}
				
				TreeItem item = (TreeItem) event.item;
				int index = tree.indexOf( item );
				int count = allFiles.size() - 1;
				
				if( index < count )
				{
					setProgress( (float)index / (float)count );
				}
				else
				{
					setProgress( 0 );
				}
				
				String filename = allFiles.get( index );
				String shortFilename = trimFront( filename, Arc.getInstance().getBaseDirectory() );
				String text = shortFilename.substring( shortFilename.lastIndexOf( File.separator ) + 1 );

				item.setData( filename );
				item.setText( 0, text );
				
				String ext = text.substring( text.lastIndexOf( '.' ) + 1 );
				item.setText( 1, ext );
				if( icons.containsKey( ext ) )
				{
					item.setImage( CreateIcon( getDisplay(), icons.getProperty( ext ) ) );
				}
				
				long modTime = Arc.getInstance().getFileModificationTime( filename );
				item.setText( 2, dateFormat.format( new Date( modTime ) ) );
			}
		} );
		tree.addListener( SWT.DefaultSelection, new Listener()
		{
			public void handleEvent( Event event )
			{
				TreeItem item = (TreeItem) event.item;
				String f = (String)item.getData();

				Arc.getInstance().addCompilerTask( f );
			}
		} );
		tree.addListener( SWT.MenuDetect, new Listener()
		{
			public void handleEvent( Event e )
			{
				final Tree t = (Tree) e.widget;
				if( t.getSelectionCount() > 0 )
				{
					final Menu clickMenu = new Menu( getShell() );
					tree.setMenu( clickMenu );
					clickMenu.setLocation( e.x, e.y );
					clickMenu.addListener( SWT.Dispose, new Listener()
					{
						public void handleEvent( Event e )
						{
							clickMenu.dispose();
						}
					} );

					MenuItem item;
					item = new MenuItem( clickMenu, SWT.PUSH );
					item.setText( "Compile" + ( t.getSelectionCount() > 1 ? "..." : "" ) );
					item.addListener( SWT.Selection, new Listener()
					{
						@Override
						public void handleEvent( Event event )
						{
							List<String> files = new ArrayList<String>( t.getSelectionCount() );
							for( TreeItem i : t.getSelection() )
							{
								files.add( (String) i.getData() );
							}
							String[] f = files.toArray( new String[ files.size() ] );
							Arc.getInstance().addCompilerTasks( f );
						}
					} );

					if( t.getSelectionCount() == 1 )
					{
						item = new MenuItem( clickMenu, SWT.PUSH );
						item.setText( "View..." );
						item.addSelectionListener( new SelectionListener()
						{

							@Override
							public void widgetSelected( SelectionEvent event )
							{
								String viewFile = (String) ( t.getSelection()[0] ).getData();
								System.out.print( viewFile );
								StringBuilder contents = new StringBuilder();
								try
								{
									String line = null;
									BufferedReader br = new BufferedReader( new InputStreamReader( new FileInputStream( viewFile ) ) );
									while( ( line = br.readLine() ) != null )
									{
										contents.append( line );
									}
								}
								catch( Exception e )
								{
									e.printStackTrace();
								}
								fileContent.setText( contents.toString() );

								textTabs.setSelection( 1 );
							}

							@Override
							public void widgetDefaultSelected( SelectionEvent e )
							{
								widgetSelected( e );
							}
						} );
					}

				}
			}
		} );
		tree.setData( Arc.getInstance().getBaseDirectory() );
		refreshTree();
		
		Arc.getInstance().addFileWatcherListener( new FileWatcherAdapter()
		{
			@Override
			public void fileListChanged()
			{
				refreshTree();
			}
		} );

		FormData fd;
		fd = new FormData();
		fd.top = fa0;
		fd.left = fa0;
		fd.right = fa100;
		search.setLayoutData( fd );

		fd = new FormData();
		fd.top = new FormAttachment( search );
		fd.left = fa0;
		fd.right = fa100;
		bar.setLayoutData( fd );

		fd = new FormData();
		fd.top = new FormAttachment( bar );
		fd.left = fa0;
		fd.right = fa100;
		fd.bottom = fa100;
		tree.setLayoutData( fd );
	}

	private void refreshTree()
	{
		Display.getDefault().asyncExec( new Runnable()
		{
			@Override
			public void run()
			{
				tree.setRedraw( false );
				tree.setItemCount( 0 );
				tree.setItemCount( Arc.getInstance().getNumFiles() );
				tree.setRedraw( true );
			}
		} );
	}

	void createTop()
	{
		comboComposite = new Composite( top, SWT.NONE );
		comboComposite.setLayout( new GridLayout( 16, false ) );

		// new Label( comboComposite, SWT.SINGLE ).setText( "Output" );
		// outputCombo = new Combo( comboComposite, SWT.READ_ONLY );
		// for( OutputLevel l : OutputLevel.values() ) {
		// outputCombo.add( l.getName() );
		// }
		// outputCombo.select( 0 );
		for( OutputLevel l : OutputLevel.values() )
		{
			Button btn = new Button( comboComposite, SWT.TOGGLE );
			btn.setText( l.name() );
		}

		new Label( comboComposite, SWT.SINGLE ).setText( "Platform" );
		platformCombo = new Combo( comboComposite, SWT.READ_ONLY );
		for( Platform p : Platform.values() )
		{
			platformCombo.add( p.name() );
		}
		platformCombo.select( 0 );
		platformCombo.addSelectionListener( new SelectionAdapter()
		{
			@Override
			public void widgetSelected( SelectionEvent event )
			{
				// Arc.getInstance().setRendering( renderings.get(
				// renderingCombo.getSelectionIndex() ) );
			}
		} );

		new Label( comboComposite, SWT.SINGLE ).setText( "Rendering" );
		renderingCombo = new Combo( comboComposite, SWT.READ_ONLY );
		for( Rendering r : Rendering.values() )
		{
			renderingCombo.add( r.name() );
		}
		renderingCombo.select( 0 );
		renderingCombo.addSelectionListener( new SelectionAdapter()
		{
			@Override
			public void widgetSelected( SelectionEvent event )
			{
				Arc.getInstance().setRendering( Rendering.values()[ renderingCombo.getSelectionIndex() ] );
			}
		} );
	}

	void createBottom()
	{
		Composite bottomComp = new Composite( bottom, SWT.NONE );

		FormLayout fl = new FormLayout();
		fl.marginTop = 3;
		fl.marginBottom = 3;
		fl.marginLeft = 3;
		fl.marginRight = 3;
		bottomComp.setLayout( fl );

		statusLabel = new Label( bottomComp, SWT.SINGLE );
		statusLabel.setText( "Status" );

		optionsLabel = new Label( bottomComp, SWT.SINGLE );
		optionsLabel.setText( "Options" );

		progress = new ProgressBar( bottomComp, SWT.HORIZONTAL | SWT.SMOOTH );
		progress.setMaximum( progressMax );
		setProgress( 0 );

		FormData fd;

		fd = new FormData();
		fd.left = fa0;
		fd.top = fa0;
		fd.bottom = fa100;
		statusLabel.setLayoutData( fd );

		fd = new FormData();
		fd.right = new FormAttachment( progress );
		fd.top = fa0;
		fd.bottom = fa100;
		optionsLabel.setLayoutData( fd );

		fd = new FormData();
		fd.right = fa100;
		fd.top = fa0;
		fd.bottom = fa100;
		progress.setLayoutData( fd );
	}

	void createText()
	{
		textTabs = new TabFolder( textComposite, SWT.NONE | SWT.BOTTOM );

		TabItem i;
		i = new TabItem( textTabs, SWT.NONE );
		i.setText( "Console" );
		console = new StyledText( textTabs, SWT.V_SCROLL | SWT.H_SCROLL | SWT.READ_ONLY );
		console.setAlwaysShowScrollBars( false );
		i.setControl( console );

		i = new TabItem( textTabs, SWT.NONE );
		i.setText( "File" );
		fileContent = new StyledText( textTabs,  SWT.V_SCROLL | SWT.H_SCROLL | SWT.READ_ONLY );
		fileContent.setAlwaysShowScrollBars( false );
		i.setControl( fileContent );
	}

	void createMenu()
	{
		menu = new Menu( getShell(), SWT.BAR );
		getShell().setMenuBar( menu );

		createFileMenu();

		createOptionsMenu();

		createCompilerMenu();
	}

	void createFileMenu()
	{
		MenuItem item;
		Menu subMenu;
		item = new MenuItem( menu, SWT.CASCADE );
		item.setText( "&File" );

		subMenu = new Menu( getShell(), SWT.DROP_DOWN );
		item.setMenu( subMenu );

		// item = new MenuItem( subMenu, SWT.PUSH );
		// item.setText( "&New\tCtrl+N" );
		// item.setAccelerator( SWT.MOD1 + 'N' );
		// item.addListener( SWT.Selection, new Listener()
		// {
		// public void handleEvent( Event e )
		// {
		//
		// }
		// } );
		//
		// item = new MenuItem( subMenu, SWT.PUSH );
		// item.setText( "&Open...\tCtrl+O" );
		// item.setAccelerator( SWT.MOD1 + 'O' );
		// item.addListener( SWT.Selection, new Listener()
		// {
		// public void handleEvent( Event e )
		// {
		//
		// }
		// } );
		//
		// new MenuItem( subMenu, SWT.SEPARATOR );
		//
		// item = new MenuItem( subMenu, SWT.PUSH );
		// item.setText( "&Close\tCtrl+W" );
		// item.setAccelerator( SWT.MOD1 + 'W' );
		// item.addListener( SWT.Selection, new Listener()
		// {
		// public void handleEvent( Event e )
		// {
		//
		// }
		// } );
		//
		// item = new MenuItem( subMenu, SWT.PUSH );
		// item.setText( "Close A&ll\tShift+Ctrl+W" );
		// item.setAccelerator( SWT.MOD1 + SWT.SHIFT + 'W' );
		// item.addListener( SWT.Selection, new Listener()
		// {
		// public void handleEvent( Event e )
		// {
		//
		// }
		// } );
		//
		// new MenuItem( subMenu, SWT.SEPARATOR );

		item = new MenuItem( subMenu, SWT.PUSH );
		item.setText( "E&xit" );
		item.addListener( SWT.Selection, new Listener()
		{
			public void handleEvent( Event e )
			{
				getShell().close();
			}
		} );
	}

	void createOptionsMenu()
	{
		MenuItem item;
		Menu subMenu;
		Menu subSubMenu;

		item = new MenuItem( menu, SWT.CASCADE );
		item.setText( "&Options" );

		subMenu = new Menu( getShell(), SWT.DROP_DOWN );
		item.setMenu( subMenu );

		item = new MenuItem( subMenu, SWT.CHECK );
		item.setText( "&Auto-Compile" );
		item.setSelection( false );
		item.setAccelerator( SWT.MOD1 + 'A' );
		item.addListener( SWT.Selection, new Listener()
		{
			public void handleEvent( Event e )
			{
				MenuItem i = (MenuItem) e.widget;
				Arc.getInstance().setAutoCompile( i.getSelection() );
				updateOptionsText();
			}
		} );

		new MenuItem( subMenu, SWT.SEPARATOR );

		item = new MenuItem( subMenu, SWT.CASCADE );
		item.setText( "Output" );

		subSubMenu = new Menu( item );
		item.setMenu( subSubMenu );

		for( final OutputLevel level : OutputLevel.values() )
		{
			item = new MenuItem( subSubMenu, SWT.RADIO );
			item.setText( "&" + level.name() );
			item.setSelection( Arc.getInstance().getOutputLevel() == level );
			item.setAccelerator( SWT.MOD1 + level.name().charAt( 0 ) );
			item.addListener( SWT.Selection, new Listener()
			{
				public void handleEvent( Event e )
				{
					Arc.getInstance().setOutputLevel( level );
					updateOptionsText();
				}
			} );
		}

		item = new MenuItem( subMenu, SWT.CASCADE );
		item.setText( "Rendering" );

		subSubMenu = new Menu( item );
		item.setMenu( subSubMenu );

		for( final Rendering rendering : Rendering.values() )
		{
			item = new MenuItem( subSubMenu, SWT.RADIO );
			item.setText( "&" + rendering.name() );
			item.setSelection( Arc.getInstance().getRendering() == rendering );
			// item.setAccelerator( SWT.MOD1 + SWT.MOD2 +
			// rendering.getName().charAt( 0 ) );
			item.addListener( SWT.Selection, new Listener()
			{
				public void handleEvent( Event e )
				{
					Arc.getInstance().setRendering( rendering );
					updateOptionsText();
				}
			} );
		}

		item = new MenuItem( subMenu, SWT.CASCADE );
		item.setText( "Platform" );

		subSubMenu = new Menu( item );
		item.setMenu( subSubMenu );

		for( final Platform platform : Platform.values() )
		{
			item = new MenuItem( subSubMenu, SWT.RADIO );
			item.setText( "&" + platform.name() );
			item.setSelection( Arc.getInstance().getPlatform() == platform );
			// item.setAccelerator( SWT.MOD1 + SWT.MOD2 +
			// rendering.platform().charAt( 0 ) );
			item.addListener( SWT.Selection, new Listener()
			{
				public void handleEvent( Event e )
				{
					Arc.getInstance().setPlatform( platform );
					updateOptionsText();
				}
			} );
		}

		item = new MenuItem( subMenu, SWT.CASCADE );
		item.setText( "Exe Mode" );

		subSubMenu = new Menu( item );
		item.setMenu( subSubMenu );

		for( final ExecutableMode executableMode : ExecutableMode.values() )
		{
			item = new MenuItem( subSubMenu, SWT.RADIO );
			item.setText( "&" + executableMode.name() );
			item.setSelection( Arc.getInstance().getExecutableMode() == executableMode );
			// item.setAccelerator( SWT.MOD1 + SWT.MOD2 +
			// rendering.platform().charAt( 0 ) );
			item.addListener( SWT.Selection, new Listener()
			{
				public void handleEvent( Event e )
				{
					Arc.getInstance().setExectuableMode( executableMode );
					updateOptionsText();
				}
			} );
		}

		new MenuItem( subMenu, SWT.SEPARATOR );

		item = new MenuItem( subMenu, SWT.PUSH );
		item.setText( "Clear Output" );
		item.addListener( SWT.Selection, new Listener()
		{
			public void handleEvent( Event event )
			{
				console.setText( "" );
				console.setStyleRange( null );
			}
		} );

		new MenuItem( subMenu, SWT.SEPARATOR );

		item = new MenuItem( subMenu, SWT.PUSH );
		item.setText( "Clean" );
		item.addListener( SWT.Selection, new Listener()
		{
			public void handleEvent( Event event )
			{
				Arc.getInstance().clean( false );
			}
		} );
	}

	void createCompilerMenu()
	{
		MenuItem item;
		Menu subMenu;
		item = new MenuItem( menu, SWT.CASCADE );
		item.setText( "&Compilers" );

		subMenu = new Menu( getShell(), SWT.DROP_DOWN );
		item.setMenu( subMenu );

		Collection<Class<? extends ArcCompiler>> compilers = Arc.getInstance().getCompilers();
		int index = 0;
		for( Class<? extends ArcCompiler> clazz : compilers )
		{
			final String compiler = clazz.getSimpleName();
			
			++index;
			item = new MenuItem( subMenu, SWT.CASCADE );
			item.setText( compiler + "\tCtrl+" + (char) ( '0' + index ) );
			item.setAccelerator( SWT.MOD1 + (char) ( '0' + index ) );
			item.addListener( SWT.Selection, new Listener()
			{
				public void handleEvent( Event event )
				{
					String[] exts = Arc.getInstance().getCompilerExtentions( compiler );
					List<String> files = Arc.getInstance().getFilesByExtensions( exts );
					
					Arc.getInstance().addCompilerTasks( files );
				}
			} );

			final Properties props = Arc.getInstance().getCompilerProperties( clazz );
			if( props != null )
			{
				Menu compilerMenu = new Menu( item );
				item.setMenu( compilerMenu );

				for( Entry<Object, Object> prop : props.entrySet() )
				{
					MenuItem compilerItem = new MenuItem( compilerMenu, SWT.PUSH );
					compilerItem.setText( prop.getKey().toString() );
				}
			}
		}

		new MenuItem( subMenu, SWT.SEPARATOR );

		item = new MenuItem( subMenu, SWT.PUSH );
		item.setText( "Reload Compilers" );
	}

	void updateTree()
	{
		tree.setItemCount( 1 );
	}

	void createDropTarget()
	{
		dropTarget = new DropTarget( this, DND.DROP_COPY | DND.DROP_DEFAULT );
		dropTarget.setTransfer( new Transfer[] { FileTransfer.getInstance() } );
		dropTarget.addDropListener( new DropTargetAdapter()
		{
			public void dragEnter( DropTargetEvent event )
			{
				if( event.detail == DND.DROP_DEFAULT )
				{
					event.detail = DND.DROP_COPY;
					System.out.println( "drag" );
				}
			}

			public void dragOperationChanged( DropTargetEvent event )
			{
				if( event.detail == DND.DROP_DEFAULT )
				{
					event.detail = DND.DROP_COPY;
					System.out.println( "changed" );
				}
			}

			public void drop( DropTargetEvent event )
			{
				if( event.data != null )
				{
					String[] a = (String[]) FileTransfer.getInstance().nativeToJava( event.dataTypes[0] );
					System.out.println( "drop " + a[0] );

				}
			}
		} );
	}

	@Override
	public void appendFromPrintOut( String text )
	{
		final String txt = text;
		Display d = Display.getDefault();
		d.asyncExec( new Runnable()
		{
			@Override
			public void run()
			{
				console.append( txt );
			}
		} );
	}

	@Override
	public void appendFromPrintErr( String text )
	{
		final String txt = text;
		Display.getDefault().asyncExec( new Runnable()
		{
			@Override
			public void run()
			{
				int offset = console.getText().length();

				console.append( txt );
				console.setStyleRange( new StyleRange( offset, txt.length(), CreateColor( getDisplay(), 255, 0, 0 ), null ) );
			}
		} );
	}

	void filterTree( String filter )
	{
		treeFilter = filter;

		Pattern p = treeFilter != null && treeFilter.length() > 0 ? Pattern.compile( ".*" + treeFilter + ".*", Pattern.CASE_INSENSITIVE ) : null;
		Color gray = CreateColor( getDisplay(), 100, 100, 100 );

		for( TreeItem i : tree.getItems() )
		{
			filterTreeItem( i, p, gray );
		}
	}

	void filterTreeItem( TreeItem i, Pattern p, Color disabledColor )
	{
		i.setForeground( p != null && !p.matcher( i.getText() ).matches() ? disabledColor : null );

		for( TreeItem ii : i.getItems() )
		{
			filterTreeItem( ii, p, disabledColor );
		}
	}

	void updateOptionsText()
	{
		StringBuffer options = new StringBuffer();
		options
		.append( "Output: " ).append( Arc.getInstance().getOutputLevel().name() )
		.append( '\t' )
		.append( "Platform: " ).append( Arc.getInstance().getPlatform().name() )
		.append( '\t' )
		.append( "Rendering: " ).append( Arc.getInstance().getRendering().name() )
		.append( '\t' )
		.append( "Exe Mode: " ).append( Arc.getInstance().getExecutableMode().name() )
		;

		optionsLabel.setText( options.toString() );
	}

	void setProgress( final float percent )
	{
		Display.getCurrent().asyncExec( new Runnable()
		{
			@Override
			public void run()
			{
				progress.setSelection( (int)( percent * progressMax ) );
			}
		});		
	}

	@Override
	public void printOut( String str )
	{
		appendFromPrintOut( str );
	}

	@Override
	public void printErr( String err )
	{
		appendFromPrintErr( err );
	}

	@Override
	public void fileChanged( String filePath )
	{
		refreshTree();
	}

	@Override
	public void fileAdded( String filePath )
	{
	}

	@Override
	public void fileRemoved( String filePath )
	{
	}

	@Override
	public void fileListChanged()
	{
		refreshTree();
	}

	@Override
	public void onArcProcessStart( int numTasks )
	{
		setProgress( 0 );
	}

	@Override
	public void onArcProcessUpdate( boolean success, int completeTime, int numTasks, int numSucces, int numError )
	{
		setProgress( (float)( numSucces + numError  ) / (float)numTasks );
	}

	@Override
	public void onArcProcessEnd()
	{
		setProgress( 0 );
	}
}
