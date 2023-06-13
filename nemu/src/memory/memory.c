#include "nemu.h"

#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({                                       \
  Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
  guest_to_host(addr);                                                        \
})

#define PDX(va) (((uint32_t)(va) >> 22) & 0x3ff)
#define PTX(va) (((uint32_t)(va) >> 12) & 0x3ff)
#define OFF(va) ((uint32_t)(va)&0xfff)
#define PTE_ADDR(pte) ((uint32_t)(pte) & ~0xfff)

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */

paddr_t page_translate(vaddr_t addr)
{
  if (cpu.cr0.PG == 0)
    return addr;
  PDE *pdt = (PDE *)PTE_ADDR(cpu.cr3);
  PDE pde;
  pde = pdt[PDX(addr)];
  pde.val = paddr_read(&pdt[PDX(addr)], 4);
  if (pde.present == 0)
  {
    panic("invalid pde:addr = %x,pde = %x", addr, pde.val);
  }

  PTE *pt = (PDE *)PTE_ADDR(pde.val);
  PTE pte;
  pte.val = paddr_read(&pt[PTX(addr)], 4);
  if (pte.present == 0)
    panic("invalid pte:addr = %x,pte = %x", addr, pte.val);

  return PTE_ADDR(pte.val) | OFF(addr);
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
  // PAGE_MASK = 0xfff
  if ((((addr) + (len)-1) & ~PAGE_MASK) != ((addr) & ~PAGE_MASK))
  {
    // data cross the page boundary
    uint32_t data = 0;
    for (int i = 0; i < len; i++)
    {
      paddr_t paddr = page_translate(addr + i);
      data += (paddr_read(paddr, 1)) << 8 * i;
    }
    return data;
    // assert(0);
  }
  else
  {
    paddr_t paddr = page_translate(addr);
    return paddr_read(paddr, len);
  }
}

void vaddr_write(vaddr_t addr, int len, uint32_t data)
{
  if ((((addr) + (len)-1) & ~PAGE_MASK) != ((addr) & ~PAGE_MASK))
  {
    // data cross the page boundary
    for (int i = 0; i < len; i++)
    { // len 最大为4
      paddr_t paddr = page_translate(addr + i);
      paddr_write(paddr, 1, data >> 8 * i);
    }
    // assert(0);
  }
  else
  {
    paddr_t paddr = page_translate(addr);
    paddr_write(paddr, len, data);
  }
}