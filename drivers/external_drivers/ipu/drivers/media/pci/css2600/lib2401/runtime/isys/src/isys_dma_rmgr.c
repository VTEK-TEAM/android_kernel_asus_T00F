/*
 * Support for Intel Camera Imaging ISP subsystem.
 *
 * Copyright (c) 2010 - 2014 Intel Corporation. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */


#include "assert_support.h"
#include "platform_support.h"
#include "ia_css_isys.h"
#include "bitop_support.h"
#include "isys_dma_rmgr.h"

static isys_dma_rsrc_t isys_dma_rsrc[N_ISYS2401_DMA_ID];

void ia_css_isys_dma_channel_rmgr_init(void)
{
	memset(&isys_dma_rsrc, 0, sizeof(isys_dma_rsrc_t));
}

void ia_css_isys_dma_channel_rmgr_uninit(void)
{
	memset(&isys_dma_rsrc, 0, sizeof(isys_dma_rsrc_t));
}

bool ia_css_isys_dma_channel_rmgr_acquire(
	isys2401_dma_ID_t	dma_id,
	isys2401_dma_channel	*channel)
{
	bool retval = false;
	isys2401_dma_channel	i;
	isys2401_dma_channel	max_dma_channel;
	isys_dma_rsrc_t		*cur_rsrc = NULL;

	assert(dma_id < N_ISYS2401_DMA_ID);
	assert(channel != NULL);

	max_dma_channel = N_ISYS2401_DMA_CHANNEL_PROCS[dma_id];
	cur_rsrc = &isys_dma_rsrc[dma_id];

	if (cur_rsrc->num_active < max_dma_channel) {
		for (i = ISYS2401_DMA_CHANNEL_0; i < N_ISYS2401_DMA_CHANNEL; i++) {
			if (bitop_getbit(cur_rsrc->active_table, i) == 0) {
				bitop_setbit(cur_rsrc->active_table, i);
				*channel = (isys2401_dma_channel)i;
				cur_rsrc->num_active++;
				retval = true;
				break;
			}
		}
	}

	return retval;
}

void ia_css_isys_dma_channel_rmgr_release(
	isys2401_dma_ID_t	dma_id,
	isys2401_dma_channel	*channel)
{
	isys2401_dma_channel	max_dma_channel;
	isys_dma_rsrc_t		*cur_rsrc = NULL;

	assert(dma_id < N_ISYS2401_DMA_ID);
	assert(channel != NULL);

	max_dma_channel = N_ISYS2401_DMA_CHANNEL_PROCS[dma_id];
	cur_rsrc = &isys_dma_rsrc[dma_id];

	if ((*channel < max_dma_channel) && (cur_rsrc->num_active > 0)) {
		if (bitop_getbit(cur_rsrc->active_table, *channel) == 1) {
			bitop_clearbit(cur_rsrc->active_table, *channel);
			cur_rsrc->num_active--;
		}
	}
}

