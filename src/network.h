/* NETWORK_H */
#ifndef NETWORK_H
#define NETWORK_H
int force_reconnect(unsigned int attempts);
int asn_reconnect(unsigned int attempts, unsigned int target_asn);
int change_ip_reconnect(unsigned int attempts);
int display_connected_devices(void);

#endif // NETWORK_H