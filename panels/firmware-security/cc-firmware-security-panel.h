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

#include <shell/cc-panel.h>

G_BEGIN_DECLS

typedef enum {
	FWUPD_STATUS_UNKNOWN,
	FWUPD_STATUS_IDLE,
	FWUPD_STATUS_LOADING,
	FWUPD_STATUS_DECOMPRESSING,
	FWUPD_STATUS_DEVICE_RESTART,
	FWUPD_STATUS_DEVICE_WRITE,
	FWUPD_STATUS_DEVICE_VERIFY,
	FWUPD_STATUS_SCHEDULING,
	FWUPD_STATUS_DOWNLOADING,
	FWUPD_STATUS_DEVICE_READ,
	FWUPD_STATUS_DEVICE_ERASE,
	FWUPD_STATUS_WAITING_FOR_AUTH,
	FWUPD_STATUS_DEVICE_BUSY,
	FWUPD_STATUS_SHUTDOWN,
	/*< private >*/
	FWUPD_STATUS_LAST
} FwupdStatus;


#define CC_TYPE_FIRMWARE_SECURITY_PANEL (cc_firmware_security_panel_get_type ())
G_DECLARE_FINAL_TYPE (CcfirmwareSecurityPanel, cc_firmware_security_panel, CC, FIRMWARE_SECURITY_PANEL, CcPanel)

G_END_DECLS
