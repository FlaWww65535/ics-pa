#include "nemu.h"

#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({                                       \
  Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
  guest_to_host(addr);                                                        \
})

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */

paddr_t page_translate(vaddr_t addr)
{
  if (cpu.cr0.PG == 0)
    return addr;
  uint32_t *pdt = (cpu.cr3);
  PDE pde;
  pde.val = pdt[addr >> 22];
  if (pde.present == 0)
    panic("invalid pde:%x", pde.val);

  uint32_t *pt = pde.page_frame << 12;
  PTE pte;
  pte.val = pt[(addr >> 12) & 0x3ff];
  if (pte.present == 0)
    panic("invalid pte:%x", pte.val);

  return (pte.page_frame << 12) + (addr & 0xfff);
}

uint32_t paddr_read(paddr_t addr, int len)
{
  int id = is_mmio(addr);
  if (id == -1)
  {
    return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
  }
  return mmio_read(addr, len, id);
}

void paddr_write(paddr_t addr, int len, uint32_t data)
{
  int id = is_mmio(addr);
  if (id == -1)
  {
    memcpy(guest_to_host(addr), &data, len);
    return;
  }
  return mmio_write(addr, len, data, id);
}

uint32_t vaddr_read(vaddr_t addr, int len)
{
  if (addr / PAGE_SIZE < (addr + len) / PAGE_SIZE)
  {
    /* this is a special case, you can handle it later. */
    panic("cross page edge\n");
  }
  else
  {
    paddr_t paddr = page_translate(addr);
    printf("read paddr %x\n", paddr);
    return paddr_read(paddr, len);
  }
}

void vaddr_write(vaddr_t addr, int len, uint32_t data)
{
  if (addr / PAGE_SIZE < (addr + len) / PAGE_SIZE)
  {
    /* this is a special case, you can handle it later. */
    panic("cross page edge\n");
  }
  else
  {
    paddr_t paddr = page_translate(addr);
    printf("write paddr %x\n", paddr);
    paddr_write(paddr, len, data);
  }
}
