/*
 * Minimal debug/trace/assert driver definitions for
 * Broadcom 802.11 Networking Adapter.
 *
 * Copyright (C) 1999-2019, Broadcom.
 *
 *      Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2 (the "GPL"),
 * available at http://www.broadcom.com/licenses/GPLv2.php, with the
 * following added to such license:
 *
 *      As a special exception, the copyright holders of this software give you
 * permission to link this software with independent modules, and to copy and
 * distribute the resulting executable under terms of your choice, provided that
 * you also meet, for each linked independent module, the terms and conditions of
 * the license of that module.  An independent module is a module which is not
 * derived from this software.  The special exception does not apply to any
 * modifications of the software.
 *
 *      Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a license
 * other than the GPL, without Broadcom's express prior written consent.
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id: wl_dbg.h 697380 2017-05-03 11:34:25Z $
 */

#ifndef _wl_dbg_h_
#define _wl_dbg_h_

#include <typedefs.h>

#if defined(EVENT_LOG_COMPILE)
#include <event_log.h>
#endif // endif

/* conditional debug flags */
#define WL_DBG_NONE 0
#define WL_DBG_P2P_ACTION (1 << 5)
#define WL_DBG_TRACE (1 << 4)
#define WL_DBG_SCAN (1 << 3)
#define WL_DBG_DBG (1 << 2)
#define WL_DBG_INFO (1 << 1)
#define WL_DBG_ERR (1 << 0)

/* wl_msg_level is a bit vector with defs in wlioctl.h */
extern uint32 wl_msg_level;
extern uint32 wl_msg_level2;

#define WL_TIMESTAMP()
#define WIFICC_CAPTURE(_reason)
#define WIFICC_LOGDEBUGIF(_flags, _args)
#define WIFICC_LOGDEBUG(_args)

/* Logger prefix -> [dhd] CFG80211: */
#define CFG80211_ERROR_TEXT DHD_LOG_PREFIXS "CFG80211[E]: "

/* Default error logging macros -> KERN_ERR */
#define WL_DEBUG_RATE_LIMIT_PERIOD 4000000000u /* 4s in units of ns */
#define WL_ERR_RLMT(args)  \
	do   \
	{ \
		if ((wl_dbg_level & WL_DBG_ERR) || (wl_msg_level & WL_ERROR_VAL)) \
		{   \
			static uint64 __err_ts = 0;   \
			static uint32 __err_cnt = 0;  \
			uint64 __cur_ts = 0; \
			__cur_ts = local_clock();  \
			if (__err_ts == 0 || (__cur_ts > __err_ts && \
				(__cur_ts - __err_ts > WL_DEBUG_RATE_LIMIT_PERIOD))) \
			{  \
				__err_ts = __cur_ts;   \
				WL_ERR(args); \
				WL_ERR(("[Repeats %u times]\n", __err_cnt));   \
				__err_cnt = 0;   \
			}  \
			else  \
			{  \
				++__err_cnt;  \
			}  \
		}   \
	} while (0)

#define WL_ERR_MSG(x, args...) \
	do \
	{  \
		if (((wl_dbg_level & WL_DBG_ERR) ||   \
			 (wl_msg_level & WL_ERROR_VAL)))   \
		{ \
			printk(KERN_ERR CFG80211_ERROR_TEXT "%s: " \
					x, __func__, ##args);  \
		} \
	} while (0)
#define WL_ERR_KERN(x) WL_ERR_MSG x
#define WL_ERR_MEM(x) WL_ERR_MSG x
#define WL_ERR_EX(x) WL_ERR_MSG x
#define WL_ERR(x) WL_ERR_MSG x
#define WL_ERROR(x) WL_ERR_MSG x

/* ------------------------------------------ */
/* Default debug logging macros -> KERN_DEBUG */
#if defined(DHD_DEBUG)

/* Logger prefix -> [dhd] CFG80211: */
#define CFG80211_INFO_TEXT DHD_LOG_PREFIXS "CFG80211[I]: "
#define CFG80211_DEBUG_TEXT DHD_LOG_PREFIXS "CFG80211[D]: "

/* Info logging macros <5> */
#define WL_INFO_MSG(x, args...)  \
	do  \
	{   \
		if (((wl_dbg_level & WL_DBG_INFO) ||   \
			 (wl_msg_level & WL_DBG_INFO))) \
		{  \
			printk(KERN_NOTICE CFG80211_INFO_TEXT "%s: " x, \
			 __func__, ##args); \
		}  \
	} while (0)
#define WL_INFORM_MEM(x) WL_INFO_MSG x
#define WL_INFORM(x) WL_INFO_MSG x
#define WL_INFO(x) WL_INFO_MSG x
#define WL_MEM(x) WL_INFO_MSG x

/* Scanner logging macros <6> */
#define WL_SCAN_MSG(x, args...) \
	do  \
	{   \
		if (((wl_dbg_level & WL_DBG_SCAN) ||   \
			 (wl_msg_level & WL_DBG_SCAN)) &&  \
			net_ratelimit()) \
		{  \
			printk(KERN_INFO CFG80211_INFO_TEXT "%s:" x, \
			__func__, ##args); \
		}  \
	} while (0)
#define WL_SCAN(x) WL_SCAN_MSG x

/* Simple debugging macros <6> */
#define WL_DEBUG_MSG(x, args...)   \
	do  \
	{   \
		if (((wl_dbg_level & WL_DBG_DBG) || \
			 (wl_msg_level & WL_DBG_DBG))) \
		{  \
			printk(KERN_INFO CFG80211_DEBUG_TEXT "%s: " x,   \
			 __func__, ##args); \
		}  \
	} while (0)
#define WL_DEBUG(x) WL_DEBUG_MSG x
#define WL_DBG(x) WL_DEBUG_MSG x

/* Tracing macros <7> */
#define WL_TRACE_MSG(x, args...)   \
	do  \
	{   \
		if (((wl_dbg_level & WL_DBG_TRACE) ||  \
			 (wl_msg_level & WL_DBG_TRACE))) \
		{  \
			printk(KERN_DEBUG CFG80211_DEBUG_TEXT "%s:" x, \
			__func__, ##args); \
		}  \
	} while (0)
#define WL_TRACE_HW4(x) WL_TRACE_MSG x
#define WL_TRACE(x) WL_TRACE_MSG x

#define WL_MSG(name, arg1, args...)  \
	do \
	{  \
		if ((wl_dbg_level & WL_DBG_TRACE))  \
		{ \
			printk(KERN_DEBUG DHD_LOG_PREFIX "[%s] %s: " arg1, 	\
			    	name, __func__, ##args);   					\
		} \
	} while (0)

#define WL_MSG_PRINT_RATE_LIMIT_PERIOD 1000000000u /* 1s in units of ns */
#define WL_MSG_RLMT(name, cmp, size, arg1, args...)  \
	do  \
	{   \
		if (wl_dbg_level & WL_DBG_TRACE)   \
		{  \
			static uint64 __err_ts = 0;  \
			static uint32 __err_cnt = 0; \
			uint64 __cur_ts = 0;   \
			static uint8 static_tmp[size];  \
			__cur_ts = local_clock(); \
			if (__err_ts == 0 || (__cur_ts > __err_ts && (__cur_ts - __err_ts > WL_MSG_PRINT_RATE_LIMIT_PERIOD)) || \
				memcmp(&static_tmp, cmp, size))   \
			{ \
				__err_ts = __cur_ts;  \
				memcpy(static_tmp, cmp, size); \
				printk(KERN_DEBUG DHD_LOG_PREFIX "[%s] %s: " arg1,   \
					   name, __func__, ##args);   \
				__err_cnt = 0;  \
			} \
			else \
			{ \
				++__err_cnt; \
			} \
		}  \
	} while (0)

#else /* defined(DHD_DEBUG) */
#define WL_INFO_MSG(x, args...)
#define WL_INFORM_MEM(x)
#define WL_INFORM(x)
#define WL_INFO(x)
#define WL_MEM(x)

#define WL_SCAN_MSG(x, args...)
#define WL_SCAN(x)

#define WL_DEBUG_MSG(x, args...)
#define WL_DEBUG(x)
#define WL_DBG(x)
#define WL_DEBUG(x)

#define WL_TRACE_MSG(x, args...)
#define WL_TRACE_HW4(x)
#define WL_TRACE(x)

#define WL_MSG_RLMT(name, cmp, size, arg1, args...)
#define WL_MSG(name, arg1, args...)

#endif /* defined(DHD_DEBUG) */

/* I don't know - free for use */
#define WL_PNO(x)
#define WL_SD(x)

#ifdef ENABLE_CORECAPTURE
#define MAX_BACKTRACE_DEPTH 32
extern int WL_DEBUG_backtrace(const char *prefix, void *i_backtrace, int i_backtrace_depth);
#else
#define WL_DEBUG_backtrace(a, b, c)
#endif /* ENABLE_CORECAPTURE */

#if defined(EVENT_LOG_COMPILE) && defined(WLMSG_SRSCAN)
#define _WL_SRSCAN(fmt, ...) EVENT_LOG(EVENT_LOG_TAG_SRSCAN, fmt, ##__VA_ARGS__)
#define WL_SRSCAN(args) _WL_SRSCAN args
#else
#define WL_SRSCAN(args)
#endif // endif

#if defined(BCMCONDITIONAL_LOGGING) && defined(BCMDBG) && defined(DHD_DEBUG)

/* Ideally this should be some include file that vendors can include to conditionalize logging */

/* DBGONLY() macro to reduce ifdefs in code for statements that are only needed when
 * BCMDBG is defined.
 */
#define DBGONLY(x) x

/* To disable a message completely ... until you need it again */
#define WL_NONE(args)
#define WL_SCAN_ERROR(args)
#define WL_IE_ERROR(args)
#define WL_AMSDU_ERROR(args)
#define WL_ASSOC_ERROR(args)
#define KM_ERR(args)
#define WL_PRHDRS_MSG(args)
#define WL_PRHDRS(i, p, f, t, r, l)
#define WL_PRPKT(m, b, n)
#define WL_TMP(args)
#define WL_OID(args)
#define WL_RATE(args) \
	do \
	{  \
		if ((wl_msg_level & WL_RATE_VAL) && net_ratelimit()) \
			WL_DEBUG(args); \
	} while (0)
#define WL_ASSOC(args) \
	do  \
	{   \
		if ((wl_msg_level & WL_ASSOC_VAL) && net_ratelimit()) \
			WL_DEBUG(args);  \
		else  \
			WIFICC_LOGDEBUG(args); \
	} while (0)
#define WL_PRUSR(m, b, n)
#define WL_PS(args) \
	do  \
	{   \
		if ((wl_msg_level & WL_PS_VAL) && net_ratelimit()) \
			WL_DEBUG(args);  \
	} while (0)

#define WL_PORT(args)
#define WL_DUAL(args)
#define WL_REGULATORY(args) \
	do \
	{  \
		if ((wl_msg_level & WL_REGULATORY_VAL) && net_ratelimit()) \
			WL_DEBUG(args); \
		else \
			WIFICC_LOGDEBUG(args);   \
	} while (0)

#define WL_MPC(args)
#define WL_APSTA(args)
#define WL_APSTA_BCN(args)
#define WL_APSTA_TX(args)
#define WL_APSTA_TSF(args)
#define WL_APSTA_BSSID(args)
#define WL_BA(args)
#define WL_MBSS(args)
#define WL_MODE_SWITCH(args)
#define WL_PROTO(args)

#define WL_CAC(args) \
	do   \
	{ \
		if ((wl_msg_level & WL_CAC_VAL) && net_ratelimit()) \
			WL_DEBUG(args);   \
	} while (0)
#define WL_AMSDU(args)
#define WL_AMPDU(args)
#define WL_FFPLD(args)
#define WL_MCHAN(args)

#define WL_DFS(args)
#define WL_WOWL(args)
#define WL_DPT(args)
#define WL_ASSOC_OR_DPT(args)
#define WL_SCAN(args)  \
	do  \
	{   \
		if ((wl_msg_level2 & WL_SCAN_VAL) && net_ratelimit()) \
			WL_DEBUG(args);  \
	} while (0)
#define WL_COEX(args)
#define WL_RTDC(w, s, i, j)
#define WL_RTDC2(w, s, i, j)
#define WL_CHANINT(args)
#define WL_BTA(args)
#define WL_P2P(args)
#define WL_ITFR(args)
#define WL_TDLS(args)
#define WL_MCNX(args)
#define WL_PROT(args)
#define WL_PSTA(args)
#define WL_WFDS(m, b, n)
#define WL_TRF_MGMT(args)
#define WL_L2FILTER(args)
#define WL_MQ(args)
#define WL_TXBF(args)
#define WL_MUMIMO(args)
#define WL_P2PO(args)
#define WL_ROAM(args)
#define WL_WNM(args)
#define WL_NAT(args)

#ifdef WLMSG_MESH
#define WL_MESH(args) WL_DEBUG(args)
#define WL_MESH_AMPE(args) WL_DEBUG(args)
#define WL_MESH_ROUTE(args) WL_DEBUG(args)
#define WL_MESH_BCN(args)
#else
#define WL_MESH(args)
#define WL_MESH_AMPE(args)
#define WL_MESH_ROUTE(args)
#define WL_MESH_BCN(args)
#endif // endif
#ifdef WLMSG_NATOE
#define WL_NAT(args) \
	do   \
	{ \
		if ((wl_msg_level2 & WL_NATOE_VAL) 
			&& net_ratelimit()) \
			WL_DEBUG(args);   \
	} while (0)
#else
#define WL_NAT(args)
#endif // endif

#define WL_PFN_ERROR(args)

#define WL_AMPDU_UPDN(args)
#define WL_AMPDU_RX(args)
#define WL_AMPDU_ERR(args)
#define WL_AMPDU_TX(args)
#define WL_AMPDU_CTL(args)
#define WL_AMPDU_HW(args)
#define WL_AMPDU_HWTXS(args)
#define WL_AMPDU_HWDBG(args)
#define WL_AMPDU_STAT(args)
#define WL_AMPDU_ERR_ON() 0
#define WL_AMPDU_HW_ON() 0
#define WL_AMPDU_HWTXS_ON() 0

#define WL_APSTA_UPDN(args)
#define WL_APSTA_RX(args)
#define WL_WSEC(args)
#define WL_WSEC_DUMP(args)
#define WL_PCIE(args)
#define WL_TSLOG(w, s, i, j)
#define WL_FBT(args)
#define WL_MBO_DBG(args)
#define WL_RANDMAC_DBG(args)
#define WL_BAM_ERR(args)
#define WL_ADPS(args)
#define WL_OCE_DBG(args)

#define WL_ERROR_ON() (wl_msg_level & WL_ERROR_VAL)
#define WL_TRACE_ON() 0
#define WL_PRHDRS_ON() 0
#define WL_PRPKT_ON() 0
#define WL_INFORM_ON() 0
#define WL_TMP_ON() 0
#define WL_OID_ON() 0
#define WL_RATE_ON() (wl_msg_level & WL_RATE_VAL)
#define WL_ASSOC_ON() (wl_msg_level & WL_ASSOC_VAL)
#define WL_PRUSR_ON() 0
#define WL_PS_ON() (wl_msg_level & WL_PS_VAL)
#define WL_PORT_ON() 0
#define WL_WSEC_ON() 0
#define WL_WSEC_DUMP_ON() 0
#define WL_MPC_ON() 0
#define WL_REGULATORY_ON() (wl_msg_level & WL_REGULATORY_VAL)
#define WL_APSTA_ON() 0
#define WL_DFS_ON() 0
#define WL_MBSS_ON() 0
#define WL_CAC_ON() (wl_msg_level & WL_CAC_VAL)
#define WL_AMPDU_ON() 0
#define WL_DPT_ON() 0
#define WL_WOWL_ON() 0
#define WL_SCAN_ON() (wl_msg_level2 & WL_SCAN_VAL)
#define WL_BTA_ON() 0
#define WL_P2P_ON() 0
#define WL_ITFR_ON() 0
#define WL_MCHAN_ON() 0
#define WL_TDLS_ON() 0
#define WL_MCNX_ON() 0
#define WL_PROT_ON() 0
#define WL_PSTA_ON() 0
#define WL_TRF_MGMT_ON() 0
#define WL_LPC_ON() 0
#define WL_L2FILTER_ON() 0
#define WL_TXBF_ON() 0
#define WL_P2PO_ON() 0
#define WL_TSLOG_ON() 0
#define WL_WNM_ON() 0
#define WL_PCIE_ON() 0
#define WL_MUMIMO_ON() 0
#define WL_MESH_ON() 0
#define WL_MBO_DBG_ON() 0
#define WL_RANDMAC_DBG_ON() 0
#define WL_ADPS_ON() 0
#define WL_OCE_DBG_ON() 0

#else /* !BCMDBG */

/* DBGONLY() macro to reduce ifdefs in code for statements that are only needed when
 * BCMDBG is defined.
 */
#define DBGONLY(x)

/* To disable a message completely ... until you need it again */
#define WL_NONE(args)
#define WL_AMPDU_ERR(args)
#define WL_APSTA_UPDN(args)
#define WL_APSTA_RX(args)
#define KM_ERR(args)

#ifdef WLMSG_WSEC
#if defined(EVENT_LOG_COMPILE) && defined(EVENT_LOG_COMPILE)
#if defined(USE_EVENT_LOG_RA)
#define WL_WSEC(args) EVENT_LOG_RA(EVENT_LOG_TAG_WL_WSEC_LOG, args)
#define WL_WSEC_DUMP(args) EVENT_LOG_RA(EVENT_LOG_TAG_WL_WSEC_DUMP, args)
#else
#define WL_WSEC(args) EVENT_LOG_FAST_CAST_PAREN_ARGS(EVENT_LOG_TAG_WL_WSEC_LOG, args)
#define WL_WSEC_DUMP(args) EVENT_LOG_FAST_CAST_PAREN_ARGS(EVENT_LOG_TAG_WL_WSEC_DUMP, args)
#endif /* USE_EVENT_LOG_RA */
#else
#define WL_WSEC(args) WL_DEBUG(args)
#define WL_WSEC_DUMP(args) WL_DEBUG(args)
#endif /* EVENT_LOG_COMPILE */
#else
#define WL_WSEC(args)
#define WL_WSEC_DUMP(args)
#endif /* WLMSG_WSEC */

#ifdef WLMSG_MBO
#if defined(EVENT_LOG_COMPILE) && defined(EVENT_LOG_COMPILE)
#if defined(USE_EVENT_LOG_RA)
#define WL_MBO_DBG(args) EVENT_LOG_RA(EVENT_LOG_TAG_MBO_DBG, args)
#define WL_MBO_INFO(args) EVENT_LOG_RA(EVENT_LOG_TAG_MBO_INFO, args)
#else
#define WL_MBO_DBG(args) \
	EVENT_LOG_COMPACT_CAST_PAREN_ARGS(EVENT_LOG_TAG_MBO_DBG, args)
#define WL_MBO_INFO(args) \
	EVENT_LOG_COMPACT_CAST_PAREN_ARGS(EVENT_LOG_TAG_MBO_INFO, args)
#endif /* USE_EVENT_LOG_RA */
#else
#define WL_MBO_DBG(args) WL_DEBUG(args)
#define WL_MBO_INFO(args) WL_DEBUG(args)
#endif /* EVENT_LOG_COMPILE */
#else
#define WL_MBO_DBG(args)
#define WL_MBO_INFO(args)
#endif /* WLMSG_MBO */

#define WL_MBO_ERR(args) WL_DEBUG(args)

#ifdef WLMSG_RANDMAC
#if defined(EVENT_LOG_COMPILE) && defined(EVENT_LOG_COMPILE)
#if defined(USE_EVENT_LOG_RA)
#define WL_RANDMAC_DBG(args) EVENT_LOG_RA(EVENT_LOG_TAG_RANDMAC_DBG, args)
#define WL_RANDMAC_INFO(args) EVENT_LOG_RA(EVENT_LOG_TAG_RANDMAC_INFO, args)
#else
#define WL_RANDMAC_DBG(args) \
	EVENT_LOG_COMPACT_CAST_PAREN_ARGS(EVENT_LOG_TAG_RANDMAC_DBG, args)
#define WL_RANDMAC_INFO(args) \
	EVENT_LOG_COMPACT_CAST_PAREN_ARGS(EVENT_LOG_TAG_RANDMAC_INFO, args)
#endif /* USE_EVENT_LOG_RA */
#else
#define WL_RANDMAC_DBG(args) WL_DEBUG(args)
#define WL_RANDMAC_INFO(args) WL_DEBUG(args)
#endif /* EVENT_LOG_COMPILE */
#else
#define WL_RANDMAC_DBG(args)
#define WL_RANDMAC_INFO(args)
#endif /* WLMSG_RANDMAC */

#define WL_RANDMAC_ERR(args) WL_DEBUG(args)

#ifdef WLMSG_OCE
#if defined(EVENT_LOG_COMPILE)
#if defined(USE_EVENT_LOG_RA)
#define WL_OCE_DBG(args) EVENT_LOG_RA(EVENT_LOG_TAG_OCE_DBG, args)
#define WL_OCE_INFO(args) EVENT_LOG_RA(EVENT_LOG_TAG_OCE_INFO, args)
#else
#define WL_OCE_DBG(args) \
	EVENT_LOG_COMPACT_CAST_PAREN_ARGS(EVENT_LOG_TAG_OCE_DBG, args)
#define WL_OCE_INFO(args) \
	EVENT_LOG_COMPACT_CAST_PAREN_ARGS(EVENT_LOG_TAG_OCE_INFO, args)
#endif /* USE_EVENT_LOG_RA */
#else
#define WL_OCE_DBG(args) WL_DEBUG(args)
#define WL_OCE_INFO(args) WL_DEBUG(args)
#endif /* EVENT_LOG_COMPILE */
#else
#define WL_OCE_DBG(args)
#define WL_OCE_INFO(args)
#endif /* WLMSG_OCE */

#define WL_OCE_ERR(args) WL_DEBUG(args)

#define WL_PCIE(args)  \
	do  \
	{   \
		if (wl_msg_level2 & WL_PCIE_VAL) \
			WL_DEBUG(args);  \
	} while (0)
#define WL_PCIE_ON() (wl_msg_level2 & WL_PCIE_VAL)
#define WL_PFN(args) \
	do   \
	{ \
		if (wl_msg_level & WL_PFN_VAL) \
			WL_DEBUG(args);   \
	} while (0)
#define WL_PFN_ON() (wl_msg_level & WL_PFN_VAL)
#endif // endif

#ifdef WLMSG_BAM
#if defined(EVENT_LOG_COMPILE)
#ifdef USE_EVENT_LOG_RA
#define WL_BAM_ERR(args) EVENT_LOG_RA(EVENT_LOG_TAG_BAM, args)
#else
#define WL_BAM_ERR(args) EVENT_LOG_COMPACT_CAST_PAREN_ARGS(EVENT_LOG_TAG_BAM, args)
#endif /* USE_EVENT_LOG_RA */
#else
#define WL_BAM_ERR(args) WL_DEBUG(args)
#endif /* EVENT_LOG_COMPILE */
#endif /* WLMSG_BAM */

#ifdef WLMSG_WNM_BSSTRANS
#if defined(EVENT_LOG_COMPILE)
#if defined(USE_EVENT_LOG_RA)
#define WL_BSSTRANS_INFO(args) EVENT_LOG_RA(EVENT_LOG_TAG_WNM_BSSTRANS_INFO, args)
#else
#define WL_BSSTRANS_INFO(args) \
	EVENT_LOG_COMPACT_CAST_PAREN_ARGS(EVENT_LOG_TAG_WNM_BSSTRANS_INFO, args)
#endif /* USE_EVENT_LOG_RA */
#else
#define WL_BSSTRANS_INFO(args) WL_DEBUG(args)
#endif /* EVENT_LOG_COMPILE */
#else
#define WL_BSSTRANS_INFO(args)
#endif /* WLMSG_WNM_BSSTRANS */

#define WL_BSSTRANS_ERR(args) WL_DEBUG(args)

#define DBGERRONLY(x)

/* --------------------------------------------- */
/* Redefine some output macros with dump feature */
#if defined(DHD_DEBUG) && defined(DHD_LOG_DUMP)

#ifdef WL_ERR_MSG
#undef WL_ERR_MSG
#endif
#ifdef WL_ERR
#undef WL_ERR
#endif

#define WL_ERR_MSG(x, args...) \
	do \
	{  \
		if ((wl_dbg_level & WL_DBG_ERR) && net_ratelimit())  \
		{ \
			printk(KERN_ERR CFG80211_ERROR_TEXT "%s : " x, __func__, ##args);  \
			DHD_LOG_DUMP_WRITE("[%s] %s: ", dhd_log_dump_get_timestamp(), __func__); \
			DHD_LOG_DUMP_WRITE(x, ##args); \
		} \
	} while (0)
#define WL_ERR(x) WL_ERR_MSG x

#ifdef WL_ERR_KERN_MSG
#undef WL_ERR_KERN_MSG
#endif
#ifdef WL_ERR_KERN
#undef WL_ERR_KERN
#endif

#define WL_ERR_KERN_MSG(x, args...) \
	do   \
	{ \
		if ((wl_dbg_level & WL_DBG_ERR) && net_ratelimit()) \
		{   \
			printk(KERN_ERR CFG80211_ERROR_TEXT "%s : " x, __func__, ##args); \
		}   \
	} while (0)
#define WL_ERR_KERN(x) WL_ERR_KERN_MSG x

#ifdef WL_ERR_MEM_MSG
#undef WL_ERR_MEM_MSG
#endif
#ifdef WL_ERR_MEM
#undef WL_ERR_MEM
#endif

#define WL_ERR_MEM_MSG(x, args...)   \
	do \
	{  \
		if ((wl_dbg_level & WL_DBG_ERR) && net_ratelimit())  \
		{ \
			DHD_LOG_DUMP_WRITE("[%s] %s: ", dhd_log_dump_get_timestamp(), __func__); \
			DHD_LOG_DUMP_WRITE(x, ##args); \
		} \
	} while (0)
#define WL_ERR_MEM(x) WL_ERR_MEM_MSG x

#ifdef WL_ERR_EX
#undef WL_ERR_EX
#endif

#define WL_ERR_EX(args)  \
	do \
	{  \
		if ((wl_dbg_level & WL_DBG_ERR) && net_ratelimit())  \
		{ \
			printk(KERN_ERR CFG80211_ERROR_TEXT "%s : " x, __func__, ##args);  \
			DHD_LOG_DUMP_WRITE_EX("[%s] %s: ", dhd_log_dump_get_timestamp(), __func__); \
			DHD_LOG_DUMP_WRITE_EX args; \
		} \
	} while (0)

#ifdef WL_INFORM_MEM_MSG
#undef WL_INFORM_MEM_MSG
#endif
#ifdef WL_INFORM_MEM
#undef WL_INFORM_MEM
#endif

#define WL_INFORM_MEM_MSG(x, args...)   \
	do \
	{  \
		if ((wl_dbg_level & WL_DBG_INFO) && net_ratelimit()) \
		{ \
			printk(KERN_INFO CFG80211_INFO_TEXT "%s : " x, __func__, ##args);  \
			DHD_LOG_DUMP_WRITE("[%s] %s: ", dhd_log_dump_get_timestamp(), __func__); \
			DHD_LOG_DUMP_WRITE(x, ##args); \
		} \
	} while (0)
#define WL_INFORM_MEM(x) WL_INFORM_MEM_MSG x

#ifdef WL_MEM
#undef WL_MEM
#endif

#define WL_MEM(args)  \
	do \
	{  \
		if (net_ratelimit())   \
		{ \
			DHD_LOG_DUMP_WRITE("[%s] %s: ", dhd_log_dump_get_timestamp(), __func__); \
			DHD_LOG_DUMP_WRITE args; \
		} \
	} while (0)

#endif /* defined(DHD_DEBUG) && defined(DHD_LOG_DUMP) */

#endif /* _wl_dbg_h_ */
