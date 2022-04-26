#include "egl.h"
#include "log.h"

struct egl_context *
create_egl_context(EGLNativeDisplayType display_id,
				   EGLNativeWindowType native_window)
{

	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLConfig config;

	EGLint fbAttribs[] = {EGL_SURFACE_TYPE,
						  EGL_WINDOW_BIT,
						  EGL_RED_SIZE,
						  1,
						  EGL_GREEN_SIZE,
						  1,
						  EGL_BLUE_SIZE,
						  1,
						  EGL_ALPHA_SIZE,
						  1,
						  EGL_RENDERABLE_TYPE,
						  EGL_OPENGL_ES2_BIT,
						  EGL_NONE};

	EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE,
							   EGL_NONE};

	struct egl_context *egl =
		(struct egl_context *) calloc(1, sizeof(struct egl_context));

	egl->display = eglGetDisplay(display_id);
	egl->native_window = native_window;

	if (egl->display == EGL_NO_DISPLAY) {
		LOG("No EGL Display...\n");
		return EGL_FALSE;
	}

	// Initialize EGL
	if (!eglInitialize(egl->display, &majorVersion, &minorVersion)) {
		LOG("No Initialisation...\n");
		return EGL_FALSE;
	}

	// Get configs
	if ((eglGetConfigs(egl->display, NULL, 0, &numConfigs) != EGL_TRUE) ||
		(numConfigs == 0)) {
		LOG("No configuration...\n");
		return EGL_FALSE;
	}

	// Choose config
	if ((eglChooseConfig(egl->display, fbAttribs, &config, 1, &numConfigs) !=
		 EGL_TRUE) ||
		(numConfigs != 1)) {
		LOG("No configuration...\n");
		return EGL_FALSE;
	}

	egl->surface =
		eglCreateWindowSurface(egl->display, config, egl->native_window, NULL);
	if (egl->surface == EGL_NO_SURFACE) {
		LOG("No surface...\n");
		return EGL_FALSE;
	}

	// Create a GL context
	egl->context =
		eglCreateContext(egl->display, config, EGL_NO_CONTEXT, contextAttribs);
	if (egl->context == EGL_NO_CONTEXT) {
		LOG("No context...\n");
		return EGL_FALSE;
	}

	// Make the context current
	if (!eglMakeCurrent(egl->display, egl->surface, egl->surface,
						egl->context)) {
		LOG("Could not make the current window current !\n");
		return EGL_FALSE;
	}

	return egl;
}