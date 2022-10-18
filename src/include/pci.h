#ifndef UAE_PCI_H
#define UAE_PCI_H

#ifdef FSUAE // NL
#include "uae/memory.h"
#endif

extern void pci_dump(int);

extern bool dkb_wildfire_pci_init(struct autoconfig_info *aci);
extern bool prometheus_init(struct autoconfig_info *aci);
extern bool cbvision_init(struct autoconfig_info *aci);
extern bool grex_init(struct autoconfig_info *aci);
extern bool mediator_init(struct autoconfig_info *aci);
extern bool mediator_init2(struct autoconfig_info *aci);
extern bool pci_expansion_init(struct autoconfig_info *aci);
extern struct pci_bridge *pci_bridge_get(void);
extern struct pci_board_state *pci_board_add(struct pci_bridge *pcib, const struct pci_board *pci, int slot, int func, struct autoconfig_info *aci, void *userdata);
extern bool pci_validate_address(uaecptr, uae_u32, bool);

#endif /* UAE_PCI_H */
