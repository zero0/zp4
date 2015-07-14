package org.zero0.zeropoint.tools.cube;

import java.io.BufferedInputStream;
import java.io.File;
import java.util.HashMap;
import java.util.Map;
import java.util.zip.ZipFile;

import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.Device;
import org.eclipse.swt.graphics.GC;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Tray;
import org.eclipse.swt.widgets.TrayItem;

public class CubeWebMain
{
	private Cube cube;
	
	private Display display;
	private Shell shell;
	private Menu menu;
	private Tray tray;
	private TrayItem trayItem;
	
	private boolean requiresRestart = false;
	
	private Map< String, Image > imageMap;
	private Map< Integer, Color > cachedColors;
	
	public Color getColor( int r, int g, int b )
	{
		Color c = null;
		Integer hash = ( r & 0xFF ) << 16 | ( g & 0xFF ) << 8 | ( b & 0xFF );
		
		if( cachedColors.containsKey( hash ) )
		{
			c = cachedColors.get( hash );
		}
		else
		{
			c = new Color( display, r, g, b );
			cachedColors.put( hash, c );
		}
		
		return c;
	}
	
	public Image getImage( String name, int width, int height )
	{
		Image img = null;
		
		if( imageMap.containsKey( name ) )
		{
			img = imageMap.get( name );
		}
		else
		{
			img = new Image( display, width, height );
			imageMap.put( name, img );
		}
		
		return img;
	}
	
	public Image getImageFromFile( String filename )
	{
		Image img = null;
		
		if( imageMap.containsKey( filename ) )
		{
			img = imageMap.get( filename );
		}
		else
		{
			int sep = filename.indexOf( ":" );
			if( sep > -1 )
			{
				try
				{
					String archive = filename.substring( 0, sep );
					String image = filename.substring( sep + 1 );

					ZipFile zip = new ZipFile( new File( archive ) );
					img = new Image( display, new BufferedInputStream( zip.getInputStream( zip.getEntry( image ) ) ) );
				}
				catch( Exception e )
				{
					e.printStackTrace();
				}
			}
			else
			{
				img = new Image( display, filename );
			}
			
			imageMap.put( filename, img );
		}
		
		return img;
	}
	
	public Image getIcon( String icon )
	{
		Image img = getImageFromFile( "res/famfamfam_silk_icons_v013.zip:icons/" + icon + ".png" );
		return img;
	}
	
	private void clearMenuItems()
	{
		int numItemsToKeepFromBottom = 0;
		
		MenuItem[] items = menu.getItems();
		for( int i = items.length - 1; i >= 0; --i )
		{
			MenuItem item = items[i];
			if( item.getStyle() == SWT.SEPARATOR )
			{
				break;
			}
			else
			{
				numItemsToKeepFromBottom++;
			}
		}
		
		for( int i = items.length - ( numItemsToKeepFromBottom + 1 ); i >= 0; --i )
		{
			MenuItem item = items[i];
			item.dispose();
		}
	}
	
	private void updateMenuItems()
	{
		for( int i = 0; i < 10; ++i )
		{
		MenuItem mi = new MenuItem (menu, SWT.PUSH);
		mi.setText( "Item" );
		mi.addListener( SWT.Selection, new Listener () {
			@Override
			public void handleEvent (Event event) {
				System.out.println( "selection " + event.widget );
			}
		} );
		}
	}
	
	private void createExitMenu()
	{
		new MenuItem( menu, SWT.SEPARATOR );
		
		MenuItem restart = new MenuItem( menu, SWT.PUSH );
		restart.setText( "Restart" );
		restart.addListener( SWT.Selection, new Listener()
		{
			@Override
			public void handleEvent( Event event )
			{
				restart();				
			}
		} );
		restart.setImage( getIcon( "arrow_refresh" ) );
		
		MenuItem exit = new MenuItem( menu, SWT.PUSH );
		exit.setText( "Exit" );
		exit.addListener( SWT.Selection, new Listener()
		{
			@Override
			public void handleEvent( Event event )
			{
				exit();
			}
		} );
		exit.setImage( getIcon( "cross" ) );
	}
	
	public void initialize()
	{
		Display.setAppName( "Cube" );
		
		display = new Display();
		
		tray = display.getSystemTray();
		
		imageMap = new HashMap< String, Image >();
		cachedColors = new HashMap< Integer, Color >();
		
		cube = new Cube();
		cube.initialize();
	}

	public void setup()
	{
		requiresRestart = false;
		
		shell = new Shell( display, SWT.NO_TRIM );
		shell.setText( "Cube" );
		shell.setBounds( 0, 0, 0, 0 );
		shell.open();
		shell.setVisible( false );

		if (tray == null)
		{
			System.out.println ("The system tray is not available");
		}
		else
		{
			menu = new Menu( shell, SWT.POP_UP );

			Image image = new Image (display, 16, 16);
			Image image2 = new Image (display, 16, 16);
			GC gc = new GC( image2 );
			gc.setBackground( display.getSystemColor( SWT.COLOR_BLACK ) );
			gc.fillRectangle( image2.getBounds() );
			gc.dispose();
			
			trayItem = new TrayItem( tray, SWT.NONE );
			trayItem.setToolTipText( "Cube" );
			
			menu.addListener (SWT.Show, new Listener () {
				@Override
				public void handleEvent (Event event) {
					clearMenuItems();

					updateMenuItems();
					
					createExitMenu();
				}
			});
			trayItem.addListener (SWT.Hide, new Listener () {
				@Override
				public void handleEvent (Event event) {
					System.out.println("hide");
				}
			});
			trayItem.addListener (SWT.Selection, new Listener () {
				@Override
				public void handleEvent (Event event) {
					System.out.println("selection");
				}
			});
			trayItem.addListener (SWT.DefaultSelection, new Listener () {
				@Override
				public void handleEvent (Event event) {
					System.out.println("default selection");
				}
			});
			
			trayItem.addListener (SWT.MenuDetect, new Listener () {
				@Override
				public void handleEvent (Event event) {
					menu.setVisible( true );
				}
			});
			

			trayItem.setImage (image2);
			trayItem.setHighlightImage (image);
		}
		
		cube.setup();
	}
	
	public void teardown()
	{
		for( Image image : imageMap.values() )
		{
			image.dispose();
		}
		imageMap.clear();
		
		for( Color color : cachedColors.values() )
		{
			color.dispose();
		}
		cachedColors.clear();
		
		cube.teardown();
		
		trayItem.dispose();
		trayItem = null;
		
		menu.dispose();
		menu = null;
		
		shell.dispose();
		shell = null;
	}
	
	public void destroy()
	{
		cube.shutdown();
		cube = null;
		
		tray.dispose();
		tray = null;
		
		display.dispose();
		display = null;
	}
	
	public void run()
	{
		do
		{
			setup();
			
			while( !shell.isDisposed() )
			{
				if( !display.readAndDispatch() )
				{
					display.sleep();
				}
			}
			
			teardown();
		}
		while( requiresRestart );
	}
	
	public void exit()
	{
		shell.close();
	}
	
	public void restart()
	{
		requiresRestart = true;
		exit();
	}
	
	/**
	 * @param args
	 */
	public static void main( String[] args )
	{
		CubeWebMain cubeMain = new CubeWebMain();
		cubeMain.initialize();
		cubeMain.run();
		cubeMain.destroy();
	}
}
