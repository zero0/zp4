/**
 * 
 */
package org.zero0.singularity.web;

import java.io.IOException;

import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.BodyTagSupport;

public class PlaceholderTag extends BodyTagSupport
{
	private static final long serialVersionUID = -7027553705053402362L;
	
	private String name;
	
	@Override
	public int doStartTag() throws JspException
	{
		return EVAL_BODY_BUFFERED;
	}
	
	@Override
	public int doAfterBody() throws JspException
	{
		return EVAL_PAGE;
	}
	
	@Override
	public int doEndTag() throws JspException
	{
		String body = (String)pageContext.getRequest().getAttribute( MasterPageTag.MASTER_PAGE_CONTENT + name );
		
		if( body == null && bodyContent != null )
		{
			body = bodyContent.getString();
		}
		
		if( body != null )
		{
			try
			{
				pageContext.getOut().append( body ).flush();
			}
			catch( IOException e )
			{
				e.printStackTrace();
			}
		}
		
		return EVAL_PAGE;
	}
	
	public final void setName( String placeholderName )
	{
		name = placeholderName;
	}
	
	public final String getName()
	{
		return name;
	}
}
