#include "zpRendering.h"
#include "zpOpenGL.h"

zpOpenGLShaderResource::zpOpenGLShaderResource() :
	m_shaderProgram( 0 ),
	m_vertexShader( 0 ),
	m_fragmentShader( 0 ),
	m_geometryShader( 0 ),
	m_vertexLayout( ZP_NULL )
{}
zpOpenGLShaderResource::~zpOpenGLShaderResource() {}

zp_bool zpOpenGLShaderResource::load() {
	zpProperties shaderProperties( getFilename() );
	if( shaderProperties.isEmpty() ) return false;

	// get the actual shader file that will be compiled
	zpString shaderFile = shaderProperties[ "shader.file" ];
	zpString root = getFilename().substring( 0, getFilename().lastIndexOf( zpFile::sep ) + 1 );
	zpStringBuffer buff( root );
	buff << shaderFile;
	shaderFile = buff.toString();

	// read the source file into a buffer
	zpFile file( shaderFile, ZP_FILE_MODE_READ );
	zpStringBuffer source;
	file.readFile( &source );

	// if the pixel shader function is defined for this shader, compile and create it
	if( shaderProperties.hasProperty( "shader.ps" ) ) {
		m_fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		const zp_char* src = source.getChars();
		zp_int len = source.length();

		// compile the shader
		glShaderSource( m_fragmentShader, 1, &src, &len );
		glCompileShader( m_fragmentShader );

		// get the shader status, if failed display log message
		zp_int status;
		glGetShaderiv( m_fragmentShader, GL_COMPILE_STATUS, &status );
		if( status == GL_FALSE ) {
			zpLog::error() << "Failed to compile Pixel Shader " << shaderFile << zpLog::endl;

			// get the length of the info message and display if there is one
			zp_int infoLength;
			glGetShaderiv( m_fragmentShader, GL_INFO_LOG_LENGTH, &infoLength );
			if( infoLength > 0 ) {
				zpArray<zp_char> log( infoLength );
				glGetShaderInfoLog( m_fragmentShader, infoLength, ZP_NULL, log.data() );

				zpLog::error() << log.data() << zpLog::endl;
			}

			glDeleteShader( m_fragmentShader );
			m_fragmentShader = 0;
		} else {
			zpLog::message() << "Pixel Shader Compiled Successfully " << shaderFile << zpLog::endl;
		}
	}

	// if the pixel shader function is defined for this shader, compile and create it
	if( shaderProperties.hasProperty( "shader.vs" ) ) {
		zpString layout = shaderProperties[ "shader.vs.layout" ];
		// @TODO: implement vertex shader layout

		m_vertexShader = glCreateShader( GL_VERTEX_SHADER );

		const zp_char* src = source.getChars();
		zp_int len = source.length();

		// compile the shader
		glShaderSource( m_vertexShader, 1, &src, &len );
		glCompileShader( m_vertexShader );

		// get the shader status, if failed display log message
		zp_int status;
		glGetShaderiv( m_vertexShader, GL_COMPILE_STATUS, &status );
		if( status == GL_FALSE ) {
			zpLog::error() << "Failed to compile Vertex Shader " << shaderFile << zpLog::endl;

			// get the length of the info message and display if there is one
			zp_int infoLength;
			glGetShaderiv( m_vertexShader, GL_INFO_LOG_LENGTH, &infoLength );
			if( infoLength > 0 ) {
				zpArray<zp_char> log( infoLength );
				glGetShaderInfoLog( m_vertexShader, infoLength, ZP_NULL, log.data() );

				zpLog::error() << log.data() << zpLog::endl;
			}

			glDeleteShader( m_vertexShader );
			m_vertexShader = 0;
		} else {
			zpLog::message() << "Vertex Shader Compiled Successfully " << shaderFile << zpLog::endl;
		}
	}

	// if both the fragment and vertex shader compiled, link them to a program
	if( m_fragmentShader && m_vertexShader ) {
		m_shaderProgram = glCreateProgram();
		glAttachShader( m_shaderProgram, m_fragmentShader );
		glAttachShader( m_shaderProgram, m_vertexShader );

		glLinkProgram( m_shaderProgram );

		zp_int status;
		glGetProgramiv( m_shaderProgram, GL_LINK_STATUS, &status );
		if( status == GL_FALSE ) {
			zp_int infoLength;
			glGetProgramiv( m_shaderProgram, GL_INFO_LOG_LENGTH, &infoLength );
			if( infoLength > 0 ) {
				zpArray<zp_char> log( infoLength );

				glGetProgramInfoLog( m_shaderProgram, infoLength, ZP_NULL, log.data() );

				zpLog::error() << log.data() << zpLog::endl;
			}
		}
	}

	return m_shaderProgram != 0;
}
void zpOpenGLShaderResource::unload() {
	if( m_shaderProgram ) {
		glUseProgram( 0 );

		glDetachShader( m_shaderProgram, m_fragmentShader );
		glDetachShader( m_shaderProgram, m_vertexShader );

		glDeleteShader( m_fragmentShader );
		glDeleteShader( m_vertexShader );

		glDeleteProgram( m_shaderProgram );

		m_shaderProgram = 0;
		m_fragmentShader = 0;
		m_vertexShader = 0;
	}
}

zpVertexLayout* zpOpenGLShaderResource::getVertexLayout() {
	return m_vertexLayout;
}

zp_uint zpOpenGLShaderResource::getShaderProgram() {
	return m_shaderProgram;
}
zp_uint zpOpenGLShaderResource::getVertexShader() {
	return m_vertexShader;
}
zp_uint zpOpenGLShaderResource::getFragmentShader() {
	return m_fragmentShader;
}
zp_uint zpOpenGLShaderResource::getGeometryShader() {
	return m_geometryShader;
}