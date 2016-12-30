#ifndef POLL_H_INCLUDE_GUARD
#define POLL_H_INCLUDE_GUARD

#ifdef __cplusplus
extern "C" {
#endif

int poll(struct pollfd* fds, unsigned nfds, int timeout);

#ifdef __cplusplus
}
#endif

#endif // POLL_H_INCLUDE_GUARD
