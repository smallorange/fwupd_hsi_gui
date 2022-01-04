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

#pragma once

#include <gtk/gtk.h>


G_BEGIN_DECLS

/**
 * FWUPD_SECURITY_ATTR_ID_ACPI_DMAR:
 *
 * Host Security ID attribute for ACPI DMAR table
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_ACPI_DMAR "org.fwupd.hsi.AcpiDmar"
/**
 * FWUPD_SECURITY_ATTR_ID_ENCRYPTED_RAM:
 *
 * Host Security ID attribute indicating encrypted RAM available
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_ENCRYPTED_RAM "org.fwupd.hsi.EncryptedRam"
/**
 * FWUPD_SECURITY_ATTR_ID_FWUPD_ATTESTATION:
 *
 * Host Security ID attribute for attesation
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_FWUPD_ATTESTATION "org.fwupd.hsi.Fwupd.Attestation"
/**
 * FWUPD_SECURITY_ATTR_ID_FWUPD_PLUGINS:
 *
 * Host Security ID attribute for plugins
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_FWUPD_PLUGINS "org.fwupd.hsi.Fwupd.Plugins"
/**
 * FWUPD_SECURITY_ATTR_ID_FWUPD_UPDATES:
 *
 * Host Security ID attribute for updates
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_FWUPD_UPDATES "org.fwupd.hsi.Fwupd.Updates"
/**
 * FWUPD_SECURITY_ATTR_ID_INTEL_BOOTGUARD_ENABLED:
 *
 * Host Security ID attribute for Intel Bootguard enabled
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_INTEL_BOOTGUARD_ENABLED "org.fwupd.hsi.IntelBootguard.Enabled"
/**
 * FWUPD_SECURITY_ATTR_ID_INTEL_BOOTGUARD_VERIFIED:
 *
 * Host Security ID attribute for Intel Bootguard verified
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_INTEL_BOOTGUARD_VERIFIED "org.fwupd.hsi.IntelBootguard.Verified"
/**
 * FWUPD_SECURITY_ATTR_ID_INTEL_BOOTGUARD_ACM:
 *
 * Host Security ID attribute for Intel Bootguard ACM
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_INTEL_BOOTGUARD_ACM "org.fwupd.hsi.IntelBootguard.Acm"
/**
 * FWUPD_SECURITY_ATTR_ID_INTEL_BOOTGUARD_POLICY:
 *
 * Host Security ID attribute for Intel Bootguard policy
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_INTEL_BOOTGUARD_POLICY "org.fwupd.hsi.IntelBootguard.Policy"
/**
 * FWUPD_SECURITY_ATTR_ID_INTEL_BOOTGUARD_OTP:
 *
 * Host Security ID attribute for Intel Bootguard OTP fuse
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_INTEL_BOOTGUARD_OTP "org.fwupd.hsi.IntelBootguard.Otp"
/**
 * FWUPD_SECURITY_ATTR_ID_INTEL_CET_ENABLED:
 *
 * Host Security ID attribute for Intel CET enabled
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_INTEL_CET_ENABLED "org.fwupd.hsi.IntelCet.Enabled"
/**
 * FWUPD_SECURITY_ATTR_ID_INTEL_CET_ACTIVE:
 *
 * Host Security ID attribute for Intel CET active
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_INTEL_CET_ACTIVE "org.fwupd.hsi.IntelCet.Active"
/**
 * FWUPD_SECURITY_ATTR_ID_INTEL_SMAP:
 *
 * Host Security ID attribute for Intel SMAP
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_INTEL_SMAP "org.fwupd.hsi.IntelSmap"
/**
 * FWUPD_SECURITY_ATTR_ID_IOMMU:
 *
 * Host Security ID attribute for IOMMU
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_IOMMU "org.fwupd.hsi.Iommu"
/**
 * FWUPD_SECURITY_ATTR_ID_KERNEL_LOCKDOWN:
 *
 * Host Security ID attribute for kernel lockdown
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_KERNEL_LOCKDOWN "org.fwupd.hsi.Kernel.Lockdown"
/**
 * FWUPD_SECURITY_ATTR_ID_KERNEL_SWAP:
 *
 * Host Security ID attribute for kernel swap
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_KERNEL_SWAP "org.fwupd.hsi.Kernel.Swap"
/**
 * FWUPD_SECURITY_ATTR_ID_KERNEL_TAINTED:
 *
 * Host Security ID attribute for kernel taint
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_KERNEL_TAINTED "org.fwupd.hsi.Kernel.Tainted"
/**
 * FWUPD_SECURITY_ATTR_ID_MEI_MANUFACTURING_MODE:
 *
 * Host Security ID attribute for Intel ME manufacturing mode
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_MEI_MANUFACTURING_MODE "org.fwupd.hsi.Mei.ManufacturingMode"
/**
 * FWUPD_SECURITY_ATTR_ID_MEI_OVERRIDE_STRAP:
 *
 * Host Security ID attribute for Intel ME override strap
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_MEI_OVERRIDE_STRAP "org.fwupd.hsi.Mei.OverrideStrap"
/**
 * FWUPD_SECURITY_ATTR_ID_MEI_VERSION:
 *
 * Host Security ID attribute for Intel ME version
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_MEI_VERSION "org.fwupd.hsi.Mei.Version"
/**
 * FWUPD_SECURITY_ATTR_ID_SPI_BIOSWE:
 *
 * Host Security ID attribute for Intel SPI BIOSWE configuration
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_SPI_BIOSWE "org.fwupd.hsi.Spi.Bioswe"
/**
 * FWUPD_SECURITY_ATTR_ID_SPI_BLE:
 *
 * Host Security ID attribute for Intel SPI BLE configuration
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_SPI_BLE "org.fwupd.hsi.Spi.Ble"
/**
 * FWUPD_SECURITY_ATTR_ID_SPI_SMM_BWP:
 *
 * Host Security ID attribute for Intel SPI SMM BWP
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_SPI_SMM_BWP "org.fwupd.hsi.Spi.SmmBwp"
/**
 * FWUPD_SECURITY_ATTR_ID_SPI_DESCRIPTOR:
 *
 * Host Security ID attribute for Intel SPI descriptor
 *
 * Since: 1.6.0
 **/
#define FWUPD_SECURITY_ATTR_ID_SPI_DESCRIPTOR "org.fwupd.hsi.Spi.Descriptor"
/**
 * FWUPD_SECURITY_ATTR_ID_SUSPEND_TO_IDLE:
 *
 * Host Security ID attribute for Suspend to Idle
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_SUSPEND_TO_IDLE "org.fwupd.hsi.SuspendToIdle"
/**
 * FWUPD_SECURITY_ATTR_ID_SUSPEND_TO_RAM:
 *
 * Host Security ID attribute for Suspend to RAM
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_SUSPEND_TO_RAM "org.fwupd.hsi.SuspendToRam"
/**
 * FWUPD_SECURITY_ATTR_ID_TPM_EMPTY_PCR:
 *
 * Host Security ID attribute for empty PCR
 *
 * Since: 1.7.2
 **/
#define FWUPD_SECURITY_ATTR_ID_TPM_EMPTY_PCR "org.fwupd.hsi.Tpm.EmptyPcr"
/**
 * FWUPD_SECURITY_ATTR_ID_TPM_RECONSTRUCTION_PCR0:
 *
 * Host Security ID attribute for TPM PCR0 reconstruction
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_TPM_RECONSTRUCTION_PCR0 "org.fwupd.hsi.Tpm.ReconstructionPcr0"
/**
 * FWUPD_SECURITY_ATTR_ID_TPM_VERSION_20:
 *
 * Host Security ID attribute for TPM 2.0
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_TPM_VERSION_20 "org.fwupd.hsi.Tpm.Version20"
/**
 * FWUPD_SECURITY_ATTR_ID_UEFI_SECUREBOOT:
 *
 * Host Security ID attribute for UEFI secure boot
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_UEFI_SECUREBOOT "org.fwupd.hsi.Uefi.SecureBoot"
/**
 * FWUPD_SECURITY_ATTR_ID_INTEL_DCI_ENABLED:
 *
 * Host Security ID attribute for Intel DCI enabled
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_INTEL_DCI_ENABLED "org.fwupd.hsi.IntelDci.Enabled"
/**
 * FWUPD_SECURITY_ATTR_ID_INTEL_DCI_LOCKED:
 *
 * Host Security ID attribute for Intel DCI locked
 *
 * Since: 1.5.0
 **/
#define FWUPD_SECURITY_ATTR_ID_INTEL_DCI_LOCKED "org.fwupd.hsi.IntelDci.Locked"
/**
 * FWUPD_SECURITY_ATTR_ID_UEFI_PK:
 *
 * Host Security ID attribute for UEFI PK
 *
 * Since: 1.5.5
 **/
#define FWUPD_SECURITY_ATTR_ID_UEFI_PK "org.fwupd.hsi.Uefi.Pk"


typedef enum {
	FWUPD_SECURITY_ATTR_FLAG_NONE = 0,
	FWUPD_SECURITY_ATTR_FLAG_SUCCESS = 1 << 0,
	FWUPD_SECURITY_ATTR_FLAG_OBSOLETED = 1 << 1,
	FWUPD_SECURITY_ATTR_FLAG_RUNTIME_UPDATES = 1 << 8,
	FWUPD_SECURITY_ATTR_FLAG_RUNTIME_ATTESTATION = 1 << 9,
	FWUPD_SECURITY_ATTR_FLAG_RUNTIME_ISSUE = 1 << 10,
} FwupdSecurityAttrFlags;

typedef enum {
	FWUPD_SECURITY_ATTR_RESULT_UNKNOWN,	  /* Since: 1.5.0 */
	FWUPD_SECURITY_ATTR_RESULT_ENABLED,	  /* Since: 1.5.0 */
	FWUPD_SECURITY_ATTR_RESULT_NOT_ENABLED,	  /* Since: 1.5.0 */
	FWUPD_SECURITY_ATTR_RESULT_VALID,	  /* Since: 1.5.0 */
	FWUPD_SECURITY_ATTR_RESULT_NOT_VALID,	  /* Since: 1.5.0 */
	FWUPD_SECURITY_ATTR_RESULT_LOCKED,	  /* Since: 1.5.0 */
	FWUPD_SECURITY_ATTR_RESULT_NOT_LOCKED,	  /* Since: 1.5.0 */
	FWUPD_SECURITY_ATTR_RESULT_ENCRYPTED,	  /* Since: 1.5.0 */
	FWUPD_SECURITY_ATTR_RESULT_NOT_ENCRYPTED, /* Since: 1.5.0 */
	FWUPD_SECURITY_ATTR_RESULT_TAINTED,	  /* Since: 1.5.0 */
	FWUPD_SECURITY_ATTR_RESULT_NOT_TAINTED,	  /* Since: 1.5.0 */
	FWUPD_SECURITY_ATTR_RESULT_FOUND,	  /* Since: 1.5.0 */
	FWUPD_SECURITY_ATTR_RESULT_NOT_FOUND,	  /* Since: 1.5.0 */
	FWUPD_SECURITY_ATTR_RESULT_SUPPORTED,	  /* Since: 1.5.0 */
	FWUPD_SECURITY_ATTR_RESULT_NOT_SUPPORTED, /* Since: 1.5.0 */
	/*< private >*/
	FWUPD_SECURITY_ATTR_RESULT_LAST
} FwupdSecurityAttrResult;

const gchar *
fu_security_attr_get_name(const gchar *appstream_id);
gboolean
firmware_security_attr_has_flag(guint64 flags, FwupdSecurityAttrFlags flag);
const gchar *
fwupd_security_attr_flag_to_string(FwupdSecurityAttrFlags flag);
const gchar *
fwupd_security_attr_result_to_string(FwupdSecurityAttrResult result);
void
load_custom_css (const char *path);
