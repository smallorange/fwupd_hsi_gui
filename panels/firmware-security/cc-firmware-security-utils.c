/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*-
 *
 * Copyright (C) 2021 Red Hat, Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Kate Hsuan <hpa@redhat.com>
 */

#include "config.h"

#include <glib/gi18n-lib.h>

#include "cc-firmware-security-utils.h"

const gchar *
fu_security_attr_get_name (const gchar *appstream_id)
{
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_SPI_BIOSWE) == 0)
    {
      /* TRANSLATORS: Title: SPI refers to the flash chip in the computer */
      return g_strdup (_("SPI write"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_SPI_BLE) == 0)
    {
      /* TRANSLATORS: Title: SPI refers to the flash chip in the computer */
      return g_strdup (_("SPI lock"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_SPI_SMM_BWP) == 0)
    {
      /* TRANSLATORS: Title: SPI refers to the flash chip in the computer */
      return g_strdup (_("SPI BIOS region"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_SPI_DESCRIPTOR) == 0)
    {
      /* TRANSLATORS: Title: SPI refers to the flash chip in the computer */
      return g_strdup (_("SPI BIOS Descriptor"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_ACPI_DMAR) == 0)
    {
      /* TRANSLATORS: Title: DMA as in https://en.wikipedia.org/wiki/DMA_attack  */
      return g_strdup (_("Pre-boot DMA protection is"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_INTEL_BOOTGUARD_ENABLED) == 0)
    {
      /* TRANSLATORS: Title: BootGuard is a trademark from Intel */
      return g_strdup (_("Intel BootGuard"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_INTEL_BOOTGUARD_VERIFIED) == 0)
    {
      /* TRANSLATORS: Title: BootGuard is a trademark from Intel,
       * verified boot refers to the way the boot process is verified */
      return g_strdup (_("Intel BootGuard verified boot"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_INTEL_BOOTGUARD_ACM) == 0)
    {
      /* TRANSLATORS: Title: BootGuard is a trademark from Intel,
       * ACM means to verify the integrity of Initial Boot Block */
      return g_strdup (_("Intel BootGuard ACM protected"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_INTEL_BOOTGUARD_POLICY) == 0)
    {
      /* TRANSLATORS: Title: BootGuard is a trademark from Intel,
       * error policy is what to do on failure */
      return g_strdup (_("Intel BootGuard error policy"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_INTEL_BOOTGUARD_OTP) == 0)
    {
      /* TRANSLATORS: Title: BootGuard is a trademark from Intel,
       * OTP = one time programmable */
      return g_strdup (_("Intel BootGuard OTP fuse"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_INTEL_CET_ENABLED) == 0)
    {
      /* TRANSLATORS: Title: CET = Control-flow Enforcement Technology,
       * enabled means supported by the processor */
      return g_strdup (_("Intel CET"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_INTEL_CET_ACTIVE) == 0)
    {
      /* TRANSLATORS: Title: CET = Control-flow Enforcement Technology,
       * active means being used by the OS */
      return g_strdup (_("Intel CET Active"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_INTEL_SMAP) == 0)
    {
      /* TRANSLATORS: Title: SMAP = Supervisor Mode Access Prevention */
      return g_strdup (_("Intel SMAP"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_ENCRYPTED_RAM) == 0)
    {
      /* TRANSLATORS: Title: Memory contents are encrypted, e.g. Intel TME */
      return g_strdup (_("Encrypted RAM"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_IOMMU) == 0)
    {
      /* TRANSLATORS: Title:
       * https://en.wikipedia.org/wiki/Input%E2%80%93output_memory_management_unit */
      return g_strdup (_("IOMMU device protection"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_KERNEL_LOCKDOWN) == 0)
    {
      /* TRANSLATORS: Title: lockdown is a security mode of the kernel */
      return g_strdup (_("Kernel lockdown"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_KERNEL_TAINTED) == 0)
    {
      /* TRANSLATORS: Title: if it's tainted or not */
      return g_strdup (_("Kernel tainted"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_KERNEL_SWAP) == 0)
    {
      /* TRANSLATORS: Title: swap space or swap partition */
      return g_strdup (_("Linux swap"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_SUSPEND_TO_RAM) == 0)
    {
      /* TRANSLATORS: Title: sleep state */
      return g_strdup (_("Suspend-to-ram"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_SUSPEND_TO_IDLE) == 0)
    {
      /* TRANSLATORS: Title: a better sleep state */
      return g_strdup (_("Suspend-to-idle"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_UEFI_PK) == 0)
    {
      /* TRANSLATORS: Title: PK is the 'platform key' for the machine */
      return g_strdup (_("UEFI platform key"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_UEFI_SECUREBOOT) == 0)
    {
      /* TRANSLATORS: Title: SB is a way of locking down UEFI */
      return g_strdup (_("Secure boot"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_TPM_EMPTY_PCR) == 0)
    {
      /* TRANSLATORS: Title: PCRs (Platform Configuration Registers) shouldn't be empty */
      return g_strdup (_("All TPM PCRs are"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_TPM_RECONSTRUCTION_PCR0) == 0)
    {
      /* TRANSLATORS: Title: the PCR is rebuilt from the TPM event log */
      return g_strdup (_("TPM PCR0 reconstruction"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_TPM_VERSION_20) == 0)
    {
      /* TRANSLATORS: Title: TPM = Trusted Platform Module */
      return g_strdup (_("TPM v2.0"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_MEI_MANUFACTURING_MODE) == 0)
    {
      /* TRANSLATORS: Title: MEI = Intel Management Engine */
      return g_strdup (_("MEI manufacturing mode"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_MEI_OVERRIDE_STRAP) == 0)
    {
      /* TRANSLATORS: Title: MEI = Intel Management Engine, and the
       * "override" is the physical PIN that can be driven to
       * logic high -- luckily it is probably not accessible to
       * end users on consumer boards */
      return g_strdup (_("MEI override"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_MEI_VERSION) == 0)
    {
      /* TRANSLATORS: Title: MEI = Intel Management Engine */
      return g_strdup (_("MEI version"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_FWUPD_UPDATES) == 0)
    {
      /* TRANSLATORS: Title: if firmware updates are available */
      return g_strdup (_("Firmware updates"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_FWUPD_ATTESTATION) == 0)
    {
      /* TRANSLATORS: Title: if we can verify the firmware checksums */
      return g_strdup (_("Firmware attestation"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_FWUPD_PLUGINS) == 0)
    {
      /* TRANSLATORS: Title: if the fwupd plugins are all present and correct */
      return g_strdup (_("fwupd plugins"));
    }
  if (g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_INTEL_DCI_ENABLED) == 0 ||
      g_strcmp0 (appstream_id, FWUPD_SECURITY_ATTR_ID_INTEL_DCI_LOCKED) == 0)
    {
      /* TRANSLATORS: Title: Direct Connect Interface (DCI) allows
       * debugging of Intel processors using the USB3 port */
      return g_strdup (_("Intel DCI debugger"));
    }

  /* we should not get here */
  return g_strdup (appstream_id);
}

gboolean
firmware_security_attr_has_flag(guint64 flags, FwupdSecurityAttrFlags flag)
{
  return (flags & flag) > 0;
}

const char *
fwupd_event_to_log (const char *appstream_id, FwupdSecurityAttrResult result)
{
  struct
  {
    const gchar                 *appstream_id;
    FwupdSecurityAttrResult      result;
    const gchar                 *text;
  } event_log_items[] = {{"org.fwupd.hsi.Iommu",
      FWUPD_SECURITY_ATTR_RESULT_ENABLED,
      /* TRANSLATORS: HSI event title */
      _("IOMMU device protection enabled")},
     {"org.fwupd.hsi.Iommu",
      FWUPD_SECURITY_ATTR_RESULT_NOT_FOUND,
      /* TRANSLATORS: HSI event title */
      _("IOMMU device protection disabled")},
     /* ------------------------------------------*/
     {"org.fwupd.hsi.Fwupd.Plugins",
      FWUPD_SECURITY_ATTR_RESULT_NOT_TAINTED,
      NULL},
     {"org.fwupd.hsi.Fwupd.Plugins",
      FWUPD_SECURITY_ATTR_RESULT_TAINTED,
      NULL},
     {"org.fwupd.hsi.Fwupd.Plugins",
      FWUPD_SECURITY_ATTR_RESULT_NOT_ENABLED,
      NULL},
     /* ------------------------------------------*/
     {"org.fwupd.hsi.Kernel.Tainted",
      FWUPD_SECURITY_ATTR_RESULT_NOT_TAINTED,
      /* TRANSLATORS: HSI event title */
      _("Kernel is no longer tainted")},
     {"org.fwupd.hsi.Kernel.Tainted",
      FWUPD_SECURITY_ATTR_RESULT_TAINTED,
      /* TRANSLATORS: HSI event title */
      _("Kernel is tainted")},
     /* ------------------------------------------*/
     {"org.fwupd.hsi.Kernel.Lockdown",
      FWUPD_SECURITY_ATTR_RESULT_NOT_ENABLED,
      /* TRANSLATORS: HSI event title */
      _("Kernel lockdown disabled")},
     {"org.fwupd.hsi.Kernel.Lockdown",
      FWUPD_SECURITY_ATTR_RESULT_ENABLED,
      /* TRANSLATORS: HSI event title */
      _("Kernel lockdown enabled")},
     /* ------------------------------------------*/
     {"org.fwupd.hsi.AcpiDmar",
      FWUPD_SECURITY_ATTR_RESULT_NOT_ENABLED,
      /* TRANSLATORS: HSI event title */
      _("Pre-boot DMA protection is disabled")},
     {"org.fwupd.hsi.AcpiDmar",
      FWUPD_SECURITY_ATTR_RESULT_ENABLED,
      /* TRANSLATORS: HSI event title */
      _("Pre-boot DMA protection is enabled")},
     /* ------------------------------------------*/
     {"org.fwupd.hsi.Uefi.SecureBoot",
      FWUPD_SECURITY_ATTR_RESULT_NOT_ENABLED,
      /* TRANSLATORS: HSI event title */
      _("Secure Boot disabled")},
     {"org.fwupd.hsi.Uefi.SecureBoot",
      FWUPD_SECURITY_ATTR_RESULT_ENABLED,
      /* TRANSLATORS: HSI event title */
      _("Secure Boot enabled")},
     /* ------------------------------------------*/
     {"org.fwupd.hsi.Tpm.EmptyPcr",
      FWUPD_SECURITY_ATTR_RESULT_VALID,
      /* TRANSLATORS: HSI event title */
      _("All TPM PCRs are valid")},
     {"org.fwupd.hsi.Tpm.EmptyPcr",
      FWUPD_SECURITY_ATTR_RESULT_NOT_VALID,
      /* TRANSLATORS: HSI event title */
      _("All TPM PCRs are now valid")},
     {"org.fwupd.hsi.Uefi.SecureBoot",
      FWUPD_SECURITY_ATTR_RESULT_VALID,
      /* TRANSLATORS: HSI event title */
      _("A TPM PCR is now an invalid value")},
     /* ------------------------------------------*/
     {"org.fwupd.hsi.Tpm.ReconstructionPcr0",
      FWUPD_SECURITY_ATTR_RESULT_NOT_VALID,
      /* TRANSLATORS: HSI event title */
      _("TPM PCR0 reconstruction is invalid")},
     {NULL, 0, NULL}};

  for(int i = 0; event_log_items[i].appstream_id != NULL; i++)
    {
      if (g_strcmp0 (appstream_id, event_log_items[i].appstream_id) == 0 &&
                     result == event_log_items[i].result)
        return g_strdup (event_log_items[i].text);
    }
  /* when we don't know what the event meant */
  return NULL;
}

void
load_custom_css (const char *path)
{
  g_autoptr (GtkCssProvider) provider = gtk_css_provider_new ();
  gtk_css_provider_load_from_resource (provider, path);
  gtk_style_context_add_provider_for_display (gdk_display_get_default (),
                                              GTK_STYLE_PROVIDER (provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);
}
