package org.zero0.singularity;

import java.io.BufferedInputStream;
import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.zip.ZipFile;

import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Tray;
import org.eclipse.swt.widgets.TrayItem;

public final class Singularity
{
	private SingularityEngine engine;
	
	private Display display;
	private Shell shell;
	private Menu menu;
	private Tray tray;
	private TrayItem trayItem;

	private Map< String, Image > imageMap;
	private Map< Integer, Color > cachedColors;
	
	private boolean requiresRestart = false;
	
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
	
	private Singularity()
	{
	}
	
	public void setup()
	{
		engine = new SingularityEngine();
		
		Display.setAppName( "Singularity" );
		
		display = new Display();
		
		shell = new Shell( display, SWT.NO_TRIM );
		shell.setText( "Singularity" );
		shell.setBounds( 0, 0, 0, 0 );
		shell.open();
		shell.setVisible( false );

		tray = display.getSystemTray();
		
		imageMap = new HashMap< String, Image >();
		cachedColors = new HashMap< Integer, Color >();
	}
	
	public void run()
	{
		do
		{
			requiresRestart = false;
			
			engine.setup();
			
			setupMenu();
			
			while( !shell.isDisposed() && !requiresRestart )
			{
				if( !display.readAndDispatch() )
				{
					display.sleep();
				}
			}
			
			teardownMenu();
			
			engine.teardown();
		}
		while( requiresRestart );
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
		
		shell.dispose();
		shell = null;
		
		tray.dispose();
		tray = null;
		
		display.dispose();
		display = null;
	}
	
	public void exit()
	{
		shell.close();
		requiresRestart = false;
	}
	
	public void restart()
	{
		requiresRestart = true;
	}
	
	private void setupMenu()
	{
		menu = new Menu( shell, SWT.POP_UP );
		
		Image image = getIcon( "weather_sun" );
		
		trayItem = new TrayItem( tray, SWT.NONE );
		trayItem.setToolTipText( "Singularity" );
		trayItem.setImage( image );
		trayItem.setHighlightImage( image );
		
		buildMenu();
	}
	
	private void teardownMenu()
	{
		trayItem.dispose();
		trayItem = null;
		
		menu.dispose();
		menu = null;
	}
	
	private void buildMenu()
	{
		menu.addListener (SWT.Show, new Listener () {
			@Override
			public void handleEvent (Event event) {
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

		buildEngineMenu();
		buildProjectMenu();
		buildExitMenu();
	}
	
	private void buildEngineMenu()
	{
		
	}
	
	private void buildProjectMenu()
	{
		List< String > projectIds = engine.getProjectIds();
		for( int i = 0, imax = projectIds.size(); i < imax; ++i )
		{
			String projId = projectIds.get( i );
			SingularityProject proj = engine.getProject( projId );
			
			MenuItem projMenu = new MenuItem( menu, SWT.PUSH );
			projMenu.setText( proj.getName() );
			projMenu.setImage( getIcon( "application" ) );
			projMenu.addListener( SWT.Selection, new Listener()
			{
				@Override
				public void handleEvent( Event event )
				{
				}
			} );
		}
	}
	
	private void buildExitMenu()
	{
		new MenuItem( menu, SWT.SEPARATOR );
		
		MenuItem restart = new MenuItem( menu, SWT.PUSH );
		restart.setText( "Restart" );
		restart.setImage( getIcon( "arrow_refresh" ) );
		restart.addListener( SWT.Selection, new Listener()
		{
			@Override
			public void handleEvent( Event event )
			{
				restart();
			}
		} );
		
		MenuItem exit = new MenuItem( menu, SWT.PUSH );
		exit.setText( "Exit" );
		exit.setImage( getIcon( "cross" ) );
		exit.addListener( SWT.Selection, new Listener()
		{
			@Override
			public void handleEvent( Event event )
			{
				exit();
			}
		} );
	}
	
	/**
	 * @param args
	 */
	public static void main( String[] args )
	{
		Singularity singularity = new Singularity();
		singularity.setup();
		singularity.run();
		singularity.teardown();
	}

}
