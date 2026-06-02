/* NETWORK_H */
#ifndef NETWORK_H
#define NETWORK_H

extern int force_reconnect(unsigned int attempts);
extern int asn_reconnect(unsigned int attempts, unsigned int target_asn);
extern int change_ip_reconnect(unsigned int attempts);
extern int display_connected_devices(void);

#endif // NETWORK_H