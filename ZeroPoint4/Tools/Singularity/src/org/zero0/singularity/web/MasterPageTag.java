package org.zero0.singularity.web;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.TagSupport;

public class MasterPageTag extends TagSupport
{
	public static final String MASTER_PAGE_CONTENT = "master-page-content-";
	
	private static final long serialVersionUID = -6679817841862334688L;
	
	private String page;
	
	@Override
	public int doStartTag() throws JspException
	{
		return EVAL_BODY_INCLUDE;
	}
	
	@Override
	public int doEndTag() throws JspException
	{
		try
		{
			pageContext.include( page );
		}
		catch( ServletException e )
		{
			e.printStackTrace();
		}
		catch( IOException e )
		{
			e.printStackTrace();
		}
		
		return EVAL_PAGE;
	}
	
	public final void setPage( String masterPage )
	{
		page = masterPage;
	}
	
	public final String getPage()
	{
		return page;
	}
}
