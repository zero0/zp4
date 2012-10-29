package org.zero0.zeropoint.tools.arc;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.SashForm;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.dnd.DND;
import org.eclipse.swt.dnd.DropTarget;
import org.eclipse.swt.dnd.DropTargetAdapter;
import org.eclipse.swt.dnd.DropTargetEvent;
import org.eclipse.swt.dnd.FileTransfer;
import org.eclipse.swt.dnd.Transfer;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.ProgressBar;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.TreeColumn;
import org.eclipse.swt.widgets.TreeItem;

public class Workspace extends Composite implements PrintOutAppender {

    static final FormAttachment fa0 = new FormAttachment( 0 );
    static final FormAttachment fa100 = new FormAttachment( 100 );

    static final List<Rendering> renderings = new ArrayList<Rendering>();
    static {
	renderings.addAll( Arrays.asList( Rendering.values() ) );
    }
    
    SashForm form;

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
    Combo outputCombo;
    Combo platformCombo;
    Combo renderingCombo;

    ProgressBar progress;

    PrintOutWrapper printOut;
    PrintOutWrapper printErr;

    public Workspace( Composite parent ) {
	super( parent, SWT.NONE );
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

	createTop();

	createSash();

	createTree();

	createText();

	createMenu();

	createDropTarget();

	createBottom();

	// printOut = new PrintOutWrapper( this );
	// printErr = new PrintOutWrapper( this );
	// System.setOut( printOut );
	// System.setErr( printErr );
    }

    void createSash() {
	form = new SashForm( middle, SWT.HORIZONTAL );
	form.setLayout( new FillLayout() );

	treeComposite = new Composite( form, SWT.BORDER );
	treeComposite.setLayout( new FillLayout() );

	textComposite = new Composite( form, SWT.NONE );
	textComposite.setLayout( new FillLayout() );

	form.setWeights( new int[] { 40, 60 } );
    }

    void createTree() {
	TreeColumn col;

	tree = new Tree( treeComposite, SWT.VIRTUAL | SWT.FULL_SELECTION );
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
		    text = file.getName();
		} else {
		    file = ( (File)parentItem.getData() ).listFiles()[parentItem.indexOf( item )];
		    text = file.getName();
		}

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
	tree.setData( new File( Arc.getInstance().getRootDirectory() ) );
	tree.setItemCount( ( (File)tree.getData() ).list().length );
    }

    void createTop() {
	comboComposite = new Composite( top, SWT.NONE );
	comboComposite.setLayout( new GridLayout( 16, false ) );

	new Label( comboComposite, SWT.SINGLE ).setText( "Output" );
	outputCombo = new Combo( comboComposite, SWT.READ_ONLY );
	for( OutputLevel l : OutputLevel.values() ) {
	    outputCombo.add( l.getName() );
	}
	outputCombo.select( 0 );

	new Label( comboComposite, SWT.SINGLE ).setText( "Platform" );
	platformCombo = new Combo( comboComposite, SWT.READ_ONLY );
	for( Platform p : Platform.values() ) {
	    platformCombo.add( p.getName() );
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
	    renderingCombo.add( r.getName() );
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

	progress = new ProgressBar( bottomComp, SWT.HORIZONTAL | SWT.SMOOTH );

	FormData fd;

	fd = new FormData();
	fd.left = fa0;
	fd.top = fa0;
	fd.bottom = fa100;
	statusLabel.setLayoutData( fd );

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

	createViewMenu();
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
	item.addSelectionListener( new SelectionAdapter() {

	    @Override
	    public void widgetSelected( SelectionEvent arg0 ) {
		// TODO Auto-generated method stub

	    }

	    @Override
	    public void widgetDefaultSelected( SelectionEvent arg0 ) {
		// TODO Auto-generated method stub

	    }
	} );

	item = new MenuItem( subMenu, SWT.PUSH );
	item.setText( "&Open...\tCtrl+O" );
	item.setAccelerator( SWT.MOD1 + 'O' );
	item.addSelectionListener( new SelectionAdapter() {

	    @Override
	    public void widgetSelected( SelectionEvent arg0 ) {
		// TODO Auto-generated method stub

	    }

	    @Override
	    public void widgetDefaultSelected( SelectionEvent arg0 ) {
		// TODO Auto-generated method stub

	    }
	} );

	new MenuItem( subMenu, SWT.SEPARATOR );

	item = new MenuItem( subMenu, SWT.PUSH );
	item.setText( "&Close\tCtrl+W" );
	item.setAccelerator( SWT.MOD1 + 'W' );
	item.addSelectionListener( new SelectionAdapter() {

	    @Override
	    public void widgetSelected( SelectionEvent arg0 ) {
		// TODO Auto-generated method stub

	    }

	    @Override
	    public void widgetDefaultSelected( SelectionEvent arg0 ) {
		// TODO Auto-generated method stub

	    }
	} );

	item = new MenuItem( subMenu, SWT.PUSH );
	item.setText( "Close A&ll\tShift+Ctrl+W" );
	item.setAccelerator( SWT.MOD1 + SWT.SHIFT + 'W' );
	item.addSelectionListener( new SelectionAdapter() {

	    @Override
	    public void widgetSelected( SelectionEvent arg0 ) {
		// TODO Auto-generated method stub

	    }

	    @Override
	    public void widgetDefaultSelected( SelectionEvent arg0 ) {
		// TODO Auto-generated method stub

	    }
	} );

	new MenuItem( subMenu, SWT.SEPARATOR );

	item = new MenuItem( subMenu, SWT.PUSH );
	item.setText( "E&xit" );
	item.addSelectionListener( new SelectionAdapter() {
	    @Override
	    public void widgetSelected( SelectionEvent event ) {
		getShell().close();
	    }
	} );
    }

    void createViewMenu() {
	MenuItem item;
	Menu subMenu;
	item = new MenuItem( menu, SWT.CASCADE );
	item.setText( "&View" );

	subMenu = new Menu( getShell(), SWT.DROP_DOWN );
	item.setMenu( subMenu );

	item = new MenuItem( subMenu, SWT.PUSH );
	item.setText( "&New\tCtrl+N" );
	item.setAccelerator( SWT.MOD1 + 'N' );
	item.addSelectionListener( new SelectionAdapter() {

	    @Override
	    public void widgetSelected( SelectionEvent arg0 ) {
		// TODO Auto-generated method stub

	    }

	    @Override
	    public void widgetDefaultSelected( SelectionEvent arg0 ) {
		// TODO Auto-generated method stub

	    }
	} );

	item = new MenuItem( subMenu, SWT.PUSH );
	item.setText( "&Open...\tCtrl+O" );
	item.setAccelerator( SWT.MOD1 + 'O' );
	item.addSelectionListener( new SelectionAdapter() {

	    @Override
	    public void widgetSelected( SelectionEvent arg0 ) {
		// TODO Auto-generated method stub

	    }

	    @Override
	    public void widgetDefaultSelected( SelectionEvent arg0 ) {
		// TODO Auto-generated method stub

	    }
	} );

	new MenuItem( subMenu, SWT.SEPARATOR );
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
}
