package org.zero0.singularity.web;

import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.BodyTagSupport;

public class ContentTag extends BodyTagSupport
{
	private static final long serialVersionUID = 7549092043740265493L;

	private String name;
	
	@Override
	public int doStartTag() throws JspException
	{
		return EVAL_BODY_BUFFERED;
	}
	
	@Override
	public int doEndTag() throws JspException
	{
		String body = bodyContent.getString();
		pageContext.getRequest().setAttribute( MasterPageTag.MASTER_PAGE_CONTENT + name, body );
		return EVAL_PAGE;
	}
	
	public final void setName( String contentName )
	{
		name = contentName;
	}
	
	public final String getName()
	{
		return name;
	}
}
