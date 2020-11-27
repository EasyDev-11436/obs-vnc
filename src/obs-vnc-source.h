#ifndef OBS_VNC_SOURCE_H
#define OBS_VNC_SOURCE_H

#include <pthread.h>

enum vnc_encodings_e
{
	ve_tight = 1,
	ve_zrle = 2,
	ve_ultra = 4,
	ve_hextile = 8,
	ve_zlib = 16,
	ve_corre = 32,
	ve_rre = 64,
	ve_raw = 128,
};

struct vncsrc_conig
{
	char *host_name;
	int host_port;
	// TODO: char *user_name;
	char *plain_passwd;
	int encodings;
	int compress;
	bool jpeg;
	int quality;
	int qosdscp;

	int skip_update_l, skip_update_r, skip_update_t, skip_update_b;
};

struct vnc_source
{
	pthread_mutex_t config_mutex;
	struct vncsrc_conig config;
	obs_source_t *source;
	volatile bool need_reconnect;
	volatile bool encoding_updated;
	volatile bool dscp_updated;
	volatile bool running;

	struct obs_source_frame frame;

	// threads
	pthread_t thread;
};

void vncsrc_thread_start(struct vnc_source *src);
void vncsrc_thread_stop(struct vnc_source *src);

#define BFREE_IF_NONNULL(x) if (x) { bfree(x); (x) = NULL; }

static inline void vncsrc_config_destroy_member(struct vncsrc_conig *c)
{
	BFREE_IF_NONNULL(c->host_name);
	BFREE_IF_NONNULL(c->plain_passwd);
}

#endif // OBS_VNC_SOURCE_H
