/**
 * configuration.h
 *
 * Author: Brane F. Gracnar
 *
 */

#include <sys/types.h>
#include <openssl/opensslv.h>

#include "vqueue.h"
#include "uthash.h"

/* Is NPN available? See openssl/opensslv.h for explanation. */
#if OPENSSL_VERSION_NUMBER >= 0x1000100fL
#define OPENSSL_WITH_NPN
#endif

/* Is ALPN available? See openssl/opensslv.h for explanation. */
#if OPENSSL_VERSION_NUMBER >= 0x1000200fL
#define OPENSSL_WITH_ALPN
#endif

#ifdef OPENSSL_WITH_ALPN
#define ALPN_NPN_PREFIX_STR "{alpn}"
#else
#ifdef OPENSSL_WITH_NPN
#define ALPN_NPN_PREFIX_STR "{npn}"
#endif
#endif

#ifdef USE_SHARED_CACHE
  #include "shctx.h"

  #ifndef MAX_SHCUPD_PEERS
    #define MAX_SHCUPD_PEERS 15
  #endif

typedef struct shcupd_peer_opt {
     char *ip;
     char *port;
} shcupd_peer_opt;

#endif

typedef enum {
    ENC_TLS,
    ENC_SSL
} ENC_TYPE;

typedef enum {
    SSL_SERVER,
    SSL_CLIENT
} PROXY_MODE;

struct cfg_cert_file {
	unsigned	magic;
#define CFG_CERT_FILE_MAGIC 0x58c280d2
	char 		*filename;
	char		*ocspfn;
	double		ocsp_mtim;
	int		mark;
	int		ocsp_vfy;
	double		mtim;
	UT_hash_handle	hh;
};

struct front_arg {
	unsigned		magic;
#define FRONT_ARG_MAGIC		0x07a16cb5
	char			*ip;
	char			*port;
	struct cfg_cert_file	*certs;
	char			*pspec;
	int			match_global_certs;
	int			sni_nomatch_abort;
	int			prefer_server_ciphers;
	char			*ciphers;
	ENC_TYPE		etype;
	int			mark;
	UT_hash_handle		hh;
};

/* configuration structure */
struct __hitch_config {
    ENC_TYPE ETYPE;
    PROXY_MODE PMODE;
    int WRITE_IP_OCTET;
    int WRITE_PROXY_LINE_V1;
    int WRITE_PROXY_LINE_V2;
    int PROXY_PROXY_LINE;
    char *ALPN_PROTOS;
    unsigned char *ALPN_PROTOS_LV;
    unsigned ALPN_PROTOS_LV_LEN;
    char *CHROOT;
    int UID;
    int GID;
    struct front_arg *LISTEN_ARGS;
    struct front_arg *LISTEN_DEFAULT;
    char *BACK_IP;
    char *BACK_PORT;
    long NCORES;
    struct cfg_cert_file *CERT_FILES;
    struct cfg_cert_file *CERT_DEFAULT;
    char *CIPHER_SUITE;
    char *ENGINE;
    int BACKLOG;
#ifdef USE_SHARED_CACHE
    int SHARED_CACHE;
    char *SHCUPD_IP;
    char *SHCUPD_PORT;
    shcupd_peer_opt SHCUPD_PEERS[MAX_SHCUPD_PEERS+1];
    char *SHCUPD_MCASTIF;
    char *SHCUPD_MCASTTTL;
#endif
    int QUIET;
    int SYSLOG;
    int SYSLOG_FACILITY;
    int TCP_KEEPALIVE_TIME;
    int DAEMONIZE;
    int PREFER_SERVER_CIPHERS;
    int BACKEND_CONNECT_TIMEOUT;
    int SSL_HANDSHAKE_TIMEOUT;
    int RECV_BUFSIZE;
    int SEND_BUFSIZE;
    char* LOG_FILENAME;
    int RING_SLOTS;
    int RING_DATA_LEN;
    char *PIDFILE;
    int SNI_NOMATCH_ABORT;
    int TEST;
    int OCSP_VFY;
    char *OCSP_DIR;
    double OCSP_RESP_TMO;
    double OCSP_CONN_TMO;
};

typedef struct __hitch_config hitch_config;

char * config_error_get (void);
hitch_config * config_new (void);
void config_destroy (hitch_config *cfg);
int config_file_parse (char *file, hitch_config *cfg);
int config_parse_cli(int argc, char **argv, hitch_config *cfg, int *rv);

int create_alpn_callback_data(hitch_config *cfg, char **error);
