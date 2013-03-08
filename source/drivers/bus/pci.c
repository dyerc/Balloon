#include <kernel.h>

uint16_t pciReadWord(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset)
{
  uint32_t address;
  uint32_t lbus = (uint32_t)bus;
  uint32_t lslot = (uint32_t)slot;
  uint32_t lfunc = (uint32_t)func;
  uint16_t tmp = 0;

  address = (unsigned long)((lbus << 16) | (lslot << 11) |
            (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

  outl(0x0CF8, (uint32_t)address);

  return ((uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF));
}

void pci_scan_device(uint8_t bus, uint8_t device)
{
  uint32_t vendor = pciReadWord(bus, device, 0, 2);
  if (vendor == 0xFFFF)
    return;

  uint32_t did = pciReadWord(bus, device, 0, 0);

  kprintf("Device: %x, Vendor: %x\n", did, vendor);
}

void pci_scan_bus(uint8_t bus)
{
  uint8_t device;
  for (device = 0; device < 32; device++)
    pci_scan_device(bus, device);
}

void pci_scan()
{
  pci_scan_bus(0);
}
