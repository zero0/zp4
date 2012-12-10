package org.zero0.zeropoint.tools.arc;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.regex.Pattern;

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
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.ProgressBar;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeColumn;
import org.eclipse.swt.widgets.TreeItem;
import org.zero0.zeropoint.tools.arc.compiler.ArcCompiler;

public class Workspace extends Composite implements PrintOutAppender, PrintErrAppender, OutputAppender {

    static final FormAttachment fa0 = new FormAttachment( 0 );
    static final FormAttachment fa100 = new FormAttachment( 100 );

    static final List<Rendering> renderings = new ArrayList<Rendering>();
    static {
	renderings.addAll( Arrays.asList( Rendering.values() ) );
    }
    
    SashForm form;

    Text search;
    Tree tree;
    StyledText text;

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

    ProgressBar progress;

    PrintOutWrapper printOut;
    PrintErrWrapper printErr;
    
    String treeFilter;
    
    public Workspace( Composite parent ) {
	super( parent, SWT.NONE );

	Arc.getInstance().initialize();
	
	setLayout( new FormLayout() );

	top = new Composite( this, SWT.NONE );
	top.setLayout( new FillLayout() );
	middle = new Composite( this, SWT.NONE );
	middle.setLayout( new FillLayout() );
	bottom = new Composite( this, SWT.NONE );
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

	//createTop();

	createSash();

	createText();
	
	createTree();

	createMenu();

	createDropTarget();

	createBottom();

	updateOptionsText();
	
	//printOut = new PrintOutWrapper( this );
	//printErr = new PrintErrWrapper( this );
	// System.setOut( printOut );
	// System.setErr( printErr );
	OutputStream out = new OutputStream() {
	    @Override
	    public void write( int b ) throws IOException {
		text.append( Character.toString( (char)b ) );
	    }
	};
	System.setOut( new PrintStream( out ) );
	System.setErr( new PrintStream( out ) );
    }

    void createSash() {
	form = new SashForm( middle, SWT.HORIZONTAL );
	form.setLayout( new FillLayout() );

	treeComposite = new Composite( form, SWT.BORDER );
	treeComposite.setLayout( new FormLayout() );

	textComposite = new Composite( form, SWT.NONE );
	textComposite.setLayout( new FillLayout() );

	form.setWeights( new int[] { 40, 60 } );
    }

    void createTree() {
	TreeColumn col;

	search = new Text( treeComposite, SWT.BORDER | SWT.SINGLE );
	search.addListener( SWT.KeyUp, new Listener() {
	    public void handleEvent( Event e ) {
		filterTree( search.getText() );
	    }
	} );
	
	tree = new Tree( treeComposite, SWT.VIRTUAL | SWT.FULL_SELECTION | SWT.MULTI );
	tree.setHeaderVisible( true );

	col = new TreeColumn( tree, SWT.LEFT );
	col.setText( "File" );
	col.setWidth( 200 );
	col.setResizable( true );
	col.setMoveable( true );

	col = new TreeColumn( tree, SWT.LEFT );
	col.setText( "Type" );
	col.setWidth( 100 );
	col.setResizable( true );
	col.setMoveable( true );

	tree.addListener( SWT.SetData, new Listener() {
	    public void handleEvent( Event event ) {
		TreeItem item = (TreeItem)event.item;
		TreeItem parentItem = item.getParentItem();
		String text = null;
		File file = null;

		if( parentItem == null ) {
		    file = ( (File)tree.getData() ).listFiles()[tree.indexOf( item )];
		} else {
		    file = ( (File)parentItem.getData() ).listFiles()[parentItem.indexOf( item )];
		}
		text = file.getName();
		
		item.setData( file );
		item.setText( 0, text );
		if( file.isDirectory() ) {
		    item.setText( 1, "" );
		    item.setItemCount( file.list().length );
		} else {
		    item.setText( 1, text.substring( text.lastIndexOf( '.' ) + 1 ) );
		}
	    }
	} );
	tree.addListener( SWT.DefaultSelection, new Listener() {
	    public void handleEvent( Event event ) {
		TreeItem item = (TreeItem)event.item;

		Arc.getInstance().addCompilerTask( ( (File)item.getData() ).getAbsolutePath() );
	    }
	} );
	tree.addListener( SWT.MenuDetect, new Listener() {
	    public void handleEvent( Event e ) {
		Tree t = (Tree)e.widget;
		if( t.getSelectionCount() > 0 ) {
		    Menu clickMenu = new Menu( tree );
		    tree.setMenu( clickMenu );
		    clickMenu.setLocation( e.x, e.y );
		    clickMenu.addListener( SWT.Hide, new Listener() {
			public void handleEvent( Event e ) {
			    e.widget.dispose();
			}
		    } );
		    
		    MenuItem item = new MenuItem( clickMenu, SWT.PUSH );
		    item.setText( "Compile" + ( t.getSelectionCount() > 1 ? "..." : "" ) );
		    
		    for( TreeItem i : t.getSelection() ) {
			Arc.getInstance().addCompilerTask( ( (File)i.getData() ).getAbsolutePath() );
		    }
		}
	    }
	} );
	tree.setData( new File( Arc.getInstance().getRootDirectory() ) );
	tree.setItemCount( ( (File)tree.getData() ).list().length );
	
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
	fd.bottom = fa100;
	tree.setLayoutData( fd );
    }

    void createTop() {
	comboComposite = new Composite( top, SWT.NONE );
	comboComposite.setLayout( new GridLayout( 16, false ) );

//	new Label( comboComposite, SWT.SINGLE ).setText( "Output" );
//	outputCombo = new Combo( comboComposite, SWT.READ_ONLY );
//	for( OutputLevel l : OutputLevel.values() ) {
//	    outputCombo.add( l.getName() );
//	}
//	outputCombo.select( 0 );
	for( OutputLevel l : OutputLevel.values() ) {
	    Button btn = new Button( comboComposite, SWT.TOGGLE );
	    btn.setText( l.name() );
	}

	new Label( comboComposite, SWT.SINGLE ).setText( "Platform" );
	platformCombo = new Combo( comboComposite, SWT.READ_ONLY );
	for( Platform p : Platform.values() ) {
	    platformCombo.add( p.name() );
	}
	platformCombo.select( 0 );
	platformCombo.addSelectionListener( new SelectionAdapter() {
	    @Override
	    public void widgetSelected( SelectionEvent event ) {
	        //Arc.getInstance().setRendering( renderings.get( renderingCombo.getSelectionIndex() ) );
	    }
	} );
	
	new Label( comboComposite, SWT.SINGLE ).setText( "Rendering" );
	renderingCombo = new Combo( comboComposite, SWT.READ_ONLY );
	for( Rendering r : Rendering.values() ) {
	    renderingCombo.add( r.name() );
	}
	renderingCombo.select( 0 );
	renderingCombo.addSelectionListener( new SelectionAdapter() {
	    @Override
	    public void widgetSelected( SelectionEvent event ) {
	        Arc.getInstance().setRendering( renderings.get( renderingCombo.getSelectionIndex() ) );
	    }
	} );
    }

    void createBottom() {
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

    void createText() {
	text = new StyledText( textComposite, SWT.VERTICAL | SWT.BORDER | SWT.READ_ONLY );
    }

    void createMenu() {
	menu = new Menu( getShell(), SWT.BAR );
	getShell().setMenuBar( menu );

	createFileMenu();

	createOptionsMenu();
	
	createCompilerMenu();
    }

    void createFileMenu() {
	MenuItem item;
	Menu subMenu;
	item = new MenuItem( menu, SWT.CASCADE );
	item.setText( "&File" );

	subMenu = new Menu( getShell(), SWT.DROP_DOWN );
	item.setMenu( subMenu );

	item = new MenuItem( subMenu, SWT.PUSH );
	item.setText( "&New\tCtrl+N" );
	item.setAccelerator( SWT.MOD1 + 'N' );
	item.addListener( SWT.Selection, new Listener() {
	    public void handleEvent( Event e ) {

	    }
	} );

	item = new MenuItem( subMenu, SWT.PUSH );
	item.setText( "&Open...\tCtrl+O" );
	item.setAccelerator( SWT.MOD1 + 'O' );
	item.addListener( SWT.Selection, new Listener() {
	    public void handleEvent( Event e ) {

	    }
	} );

	new MenuItem( subMenu, SWT.SEPARATOR );

	item = new MenuItem( subMenu, SWT.PUSH );
	item.setText( "&Close\tCtrl+W" );
	item.setAccelerator( SWT.MOD1 + 'W' );
	item.addListener( SWT.Selection, new Listener() {
	    public void handleEvent( Event e ) {

	    }
	} );

	item = new MenuItem( subMenu, SWT.PUSH );
	item.setText( "Close A&ll\tShift+Ctrl+W" );
	item.setAccelerator( SWT.MOD1 + SWT.SHIFT + 'W' );
	item.addListener( SWT.Selection, new Listener() {
	    public void handleEvent( Event e ) {

	    }
	} );

	new MenuItem( subMenu, SWT.SEPARATOR );

	item = new MenuItem( subMenu, SWT.PUSH );
	item.setText( "E&xit" );
	item.addListener( SWT.Selection, new Listener() {
	    public void handleEvent( Event e ) {
		getShell().close();
	    }
	} );
    }

    void createOptionsMenu() {
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
	item.addListener( SWT.Selection, new Listener() {
	    public void handleEvent( Event e ) {
		Arc.getInstance().setAutoCompile( ( (MenuItem)e.item ).getSelection() );
		updateOptionsText();
	    }
	} );

	new MenuItem( subMenu, SWT.SEPARATOR );
	
	item = new MenuItem( subMenu, SWT.CASCADE );
	item.setText( "Output" );
	
	subSubMenu = new Menu( item );
	item.setMenu( subSubMenu );
	
	for( final OutputLevel level : OutputLevel.values() ) {
	    item = new MenuItem( subSubMenu, SWT.RADIO );
	    item.setText( "&" + level.name() );
	    item.setSelection( Arc.getInstance().getOutputLevel() == level );
	    item.setAccelerator( SWT.MOD1 + level.name().charAt( 0 ) );
	    item.addListener( SWT.Selection, new Listener() {
	        public void handleEvent( Event e ) {
		    Arc.getInstance().setOutputLevel( level );
		    updateOptionsText();
		}
	    } );
	}
	
	item = new MenuItem( subMenu, SWT.CASCADE );
	item.setText( "Rendering" );
	
	subSubMenu = new Menu( item );
	item.setMenu( subSubMenu );
	
	for( final Rendering rendering : Rendering.values() ) {
	    item = new MenuItem( subSubMenu, SWT.RADIO );
	    item.setText( "&" + rendering.name() );
	    item.setSelection( Arc.getInstance().getRendering() == rendering );
	    //item.setAccelerator( SWT.MOD1 + SWT.MOD2 + rendering.getName().charAt( 0 ) );
	    item.addListener( SWT.Selection, new Listener() {
	        public void handleEvent( Event e ) {
		    Arc.getInstance().setRendering( rendering );
		    updateOptionsText();
		}
	    } );
	}
	
	item = new MenuItem( subMenu, SWT.CASCADE );
	item.setText( "Platform" );
	
	subSubMenu = new Menu( item );
	item.setMenu( subSubMenu );
	
	for( final Platform platform : Platform.values() ) {
	    item = new MenuItem( subSubMenu, SWT.RADIO );
	    item.setText( "&" + platform.name() );
	    item.setSelection( Arc.getInstance().getPlatform() == platform );
	    //item.setAccelerator( SWT.MOD1 + SWT.MOD2 + rendering.platform().charAt( 0 ) );
	    item.addListener( SWT.Selection, new Listener() {
	        public void handleEvent( Event e ) {
	            Arc.getInstance().setPlatform( platform );
		    updateOptionsText();
	        }
	    } );
	}
	
	new MenuItem( subMenu, SWT.SEPARATOR );
	
	item = new MenuItem( subMenu, SWT.PUSH );
	item.setText( "Clear Output" );
	item.addListener( SWT.Selection, new Listener() {
	    public void handleEvent( Event event ) {
		text.setText( "" );
		text.setStyleRange( null );
	    }
	} );
    }

    void createCompilerMenu() {
	MenuItem item;
	Menu subMenu;
	item = new MenuItem( menu, SWT.CASCADE );
	item.setText( "&Compilers" );

	subMenu = new Menu( getShell(), SWT.DROP_DOWN );
	item.setMenu( subMenu );

	Collection<Class<? extends ArcCompiler>> compilers = Arc.getInstance().getCompilers();
	int index = 0;
	for( Class<? extends ArcCompiler> clazz : compilers ) {
	    ++index;
	    item = new MenuItem( subMenu, SWT.CASCADE );
	    item.setText( clazz.getSimpleName() + "\tCtrl+" + (char)( '0' + index ) );
	    item.setAccelerator( SWT.MOD1 + (char)( '0' + index ) );
	    item.addListener( SWT.Selection, new Listener() {
	        public void handleEvent( Event event ) {
	    	
	        }
	    } );
	    
	    Properties props = Arc.getInstance().getCompilerProperties( clazz );
	    if( props != null ) {
		Menu compilerMenu = new Menu( item );
		item.setMenu( compilerMenu );
		
		for( Entry<Object, Object> prop : props.entrySet() ) {
		    MenuItem compilerItem = new MenuItem( compilerMenu, SWT.PUSH );
		    compilerItem.setText( prop.getKey().toString() );
		}
	    }
	}
	
	new MenuItem( subMenu, SWT.SEPARATOR );
	
	item = new MenuItem( subMenu, SWT.PUSH );
	item.setText( "Reload Compilers" );
    }
    
    void updateTree() {
	tree.setItemCount( 1 );
    }

    void createDropTarget() {
	dropTarget = new DropTarget( this, DND.DROP_COPY | DND.DROP_DEFAULT );
	dropTarget.setTransfer( new Transfer[] { FileTransfer.getInstance() } );
	dropTarget.addDropListener( new DropTargetAdapter() {
	    public void dragEnter( DropTargetEvent event ) {
		if( event.detail == DND.DROP_DEFAULT ) {
		    event.detail = DND.DROP_COPY;
		    System.out.println( "drag" );
		}
	    }

	    public void dragOperationChanged( DropTargetEvent event ) {
		if( event.detail == DND.DROP_DEFAULT ) {
		    event.detail = DND.DROP_COPY;
		    System.out.println( "changed" );
		}
	    }

	    public void drop( DropTargetEvent event ) {
		if( event.data != null ) {
		    String[] a = (String[])FileTransfer.getInstance().nativeToJava( event.dataTypes[0] );
		    System.out.println( "drop " + a[0] );

		}
	    }
	} );
    }

    @Override
    public void appendFromPrintOut( String text ) {
	this.text.append( text );
    }
    
    @Override
    public void appendFromPrintErr( String text ) {
	int offset = this.text.getText().length();
	
	this.text.append( text );
	this.text.setStyleRange( new StyleRange( offset, text.length(), new Color( getDisplay(), 255, 0, 0 ), null ) );
    }

    void filterTree( String filter ) {
	treeFilter = filter;
	
	Pattern p = treeFilter != null && treeFilter.length() > 0 ? Pattern.compile( ".*" + treeFilter + ".*", Pattern.CASE_INSENSITIVE ) : null;
	Color gray = new Color( getDisplay(), 100, 100, 100 );

	for( TreeItem i : tree.getItems() ) {
	    filterTreeItem( i, p, gray );
	}
    }

    void filterTreeItem( TreeItem i, Pattern p, Color disabledColor ) {
	i.setForeground( p != null && !p.matcher( i.getText() ).matches() ? disabledColor : null );
	
	for( TreeItem ii : i.getItems() ) {
	    filterTreeItem( ii, p, disabledColor );
	}
    }
    
    void updateOptionsText() {
	StringBuffer options = new StringBuffer();
	options.append( "Output: " ).append( Arc.getInstance().getOutputLevel().name() ).append( '\t' );
	options.append( "Platform: " ).append( Arc.getInstance().getPlatform().name() ).append( '\t' );
	options.append( "Rendering: " ).append( Arc.getInstance().getRendering().name() );
	
	optionsLabel.setText( options.toString() );
    }

    @Override
    public void printOut( String str ) {
	appendFromPrintOut( str );
    }

    @Override
    public void printErr( String err ) {
	appendFromPrintErr( err );
    }
}
