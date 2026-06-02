/**
 * ============================================================================
 * @file     ct_csr.h
 * @brief    CMSIS-Style Header File for ct_csr
 * @version  V2.0
 * @date     2025-12-01 09:51:19
 *
 * @note     Auto-generated from SystemRDL
 *           DO NOT EDIT MANUALLY - Changes will be overwritten!
 *
 * ============================================================================
 * Enumeration Handling:
 * ============================================================================
 *
 * C bitfields cannot use enum types directly (C language constraint).
 * This header provides three access methods:
 *
 * 1. Direct bitfield access (CMSIS-style):
 *    PERIPHERAL->Register.b.Field = value;  // uint32_t value
 *
 * 2. Type-safe enum setters:
 *    CT_CSR_REGISTER_SET_FIELD(&PERIPHERAL->Register, ENUM_VALUE);
 *
 * 3. Type-safe enum getters:
 *    enum_type_e val = CT_CSR_REGISTER_GET_FIELD(&PERIPHERAL->Register);
 *
 * The enum type for each field is documented in comments and provides
 * full IDE support (autocomplete, type checking, documentation).
 *
 * ============================================================================
 */

#ifndef CT_CSR_H
#define CT_CSR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* =========================================================================
   CMSIS definitions
   ========================================================================= */

#ifndef __IM
  #define __IM   volatile const  /**< Defines 'read only' permissions */
#endif
#ifndef __OM
  #define __OM   volatile        /**< Defines 'write only' permissions */
#endif
#ifndef __IOM
  #define __IOM  volatile        /**< Defines 'read/write' permissions */
#endif

/* =========================================================================
   Enumerations
   ========================================================================= */

/**
 * @brief trActCapStCmd_Cmd
 * @details HSI command
 */
typedef enum {
  CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_NONE             =  0, /**< Do nothing. */
  CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_DAQ_PC_CURR      =  1, /**< Transmit DAQ message containing the current program counter (PC), plus direct data[23:0] if value is non-negative. */
  CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_DAQ_PC_CURR_LAST =  2, /**< Transmit DAQ message with current PC, previous PC before exception or interrupt, plus direct data[23:0] if value is non-negative. */
  CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_DAQ_DIRECT_DATA  =  3, /**< Transmit DAQ message with direct data value. */
  CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_DAQ_DATA         =  4, /**< Transmit DAQ message with data value and context (data size, data type) of the next data access, plus direct data[23:0] if value is non-negative. */
  CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_DAQ_DADDR        =  5, /**< Transmit DAQ message with data address and context (data size, data type) for the next data access, plus direct data[23:0] if value is non-negative. */
  CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_DAQ_DATA_DADDR   =  6, /**< Transmit DAQ message with data value, data address, and context (data size, data type) for the next data access, plus direct data[23:0] if value is non-negative. */
  CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_DAQ_IFETCH_TH    =  8, /**< Transmit DAQ message with count of instruction fetches exceeding cycle threshold per region [direct_data[7:0]], ACT_CAP_ST plus direct data[23:8] if value is non-negative, then reset the counter. */
  CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_DAQ_DATA_RD_TH   =  9, /**< Transmit DAQ message with count of read accesses exceeding cycle threshold per region [direct_data[7:0]], ACT_CAP_ST plus direct data[23:8] if value is non-negative, then reset the counter. */
  CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_DAQ_DATA_WR      = 10, /**< Transmit DAQ message with count of write accesses for region [direct_data[7:0]], ACT_CAP_ST plus direct data[23:8] if value is non-negative, then reset the counter. */
  CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_DAQ_DATA_RD      = 11, /**< Transmit DAQ message with count of read accesses for region [direct_data[7:0]], ACT_CAP_ST plus direct data[23:8] if value is non-negative, then reset the counter. */
  CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_CF_SYNC          = 12, /**< Transmit instruction synchronization message (Nexus message only). */
  CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_TE               = 13  /**< Set TE field[direct data[7:0]], value: direct data[23:8] // TE field 0: trTeInstSyncReq (write 1 for sync request, single pulse) // TE field 1: trTe... */
} ct_csr_trActCapStCmd_Cmd_e;

/**
 * @brief trActCapStCmd_Sink
 * @details Sink
 */
typedef enum {
  CT_CSR_TRACTCAPSTCMD_SINK_ACT_CAP_ST_SINK_NEXUS      =  0, /**< Nexus message */
  CT_CSR_TRACTCAPSTCMD_SINK_ACT_CAP_ST_SINK_AXIS       =  1, /**< AXIS to internal CPU */
  CT_CSR_TRACTCAPSTCMD_SINK_ACT_CAP_ST_SINK_AXIS_NEXUS =  2, /**< AXIS to internal CPU, and Nexus message */
  CT_CSR_TRACTCAPSTCMD_SINK_ACT_CAP_ST_SINK_TE         =  3  /**< set TE register only) */
} ct_csr_trActCapStCmd_Sink_e;

/* =========================================================================
   Register Bitfield Structures
   ========================================================================= */

/**
 * @brief trActCapStCmd - ACT-CAP/ST Control Command at CSR 0x0B10 (ACT-CAP) and/or ACT-ST
 * @details Offset: 0x0B10, Access: RW
 *
 * @note Fields with enumeration types:
 *   - Cmd: ct_csr_trActCapStCmd_Cmd_e
 *   - Sink: ct_csr_trActCapStCmd_Sink_e
 */
typedef union {
  struct {
    uint32_t Cmd: 6;  /**< [5:0] - HSI command - @see ct_csr_trActCapStCmd_Cmd_e */
    uint32_t Sink: 2;  /**< [7:6] - Sink - @see ct_csr_trActCapStCmd_Sink_e */
    uint32_t DirectData:24;  /**< [31:8] - Direct data */
  } b;  /**< Bitfield access */
  uint32_t w;  /**< Word access */
} CT_CSR_trActCapStCmd_Type;

/* =========================================================================
   CT_CSR Peripheral Register Structure
   ========================================================================= */

/**
 * @brief ct_csr Peripheral Registers
 */
typedef struct {
  uint32_t RESERVED0[708];  /**< Reserved */
  __IOM CT_CSR_trActCapStCmd_Type trActCapStCmd;  /**< 0x0B10 ACT-CAP/ST Control Command at CSR 0x0B10 (ACT-CAP) and/or ACT-ST */
} CT_CSR_Type;

/* =========================================================================
   Register Bit Definitions
   ========================================================================= */

/* -------  trActCapStCmd Register  ------- */
#define CT_CSR_TRACTCAPSTCMD_CMD_Pos   0U
#define CT_CSR_TRACTCAPSTCMD_CMD_Msk   (0x3FUL << CT_CSR_TRACTCAPSTCMD_CMD_Pos)

#define CT_CSR_TRACTCAPSTCMD_SINK_Pos   6U
#define CT_CSR_TRACTCAPSTCMD_SINK_Msk   (0x3UL << CT_CSR_TRACTCAPSTCMD_SINK_Pos)

#define CT_CSR_TRACTCAPSTCMD_DIRECTDATA_Pos   8U
#define CT_CSR_TRACTCAPSTCMD_DIRECTDATA_Msk   (0xFFFFFFUL << CT_CSR_TRACTCAPSTCMD_DIRECTDATA_Pos)

/* =========================================================================
   Type-Safe Enum Accessor Macros
   ========================================================================= */

/* Type-safe enum access for trActCapStCmd */
/** @brief Set Cmd field with type-safe enum */
#define CT_CSR_TRACTCAPSTCMD_SET_CMD(reg, val) \
  ((reg)->b.Cmd = (uint32_t)(val))

/** @brief Get Cmd field as enum */
#define CT_CSR_TRACTCAPSTCMD_GET_CMD(reg) \
  ((ct_csr_trActCapStCmd_Cmd_e)((reg)->b.Cmd))

/** @brief Set Sink field with type-safe enum */
#define CT_CSR_TRACTCAPSTCMD_SET_SINK(reg, val) \
  ((reg)->b.Sink = (uint32_t)(val))

/** @brief Get Sink field as enum */
#define CT_CSR_TRACTCAPSTCMD_GET_SINK(reg) \
  ((ct_csr_trActCapStCmd_Sink_e)((reg)->b.Sink))


/* =========================================================================
   Helper Macros
   ========================================================================= */

#define SET_BIT(REG, MSK)           ((REG) |= (MSK))
#define CLEAR_BIT(REG, MSK)         ((REG) &= ~(MSK))
#define READ_BIT(REG, MSK)          ((REG) & (MSK))
#define MODIFY_REG(REG, MSK, VAL)   ((REG) = (((REG) & ~(MSK)) | ((VAL) & (MSK))))

#ifdef __cplusplus
}
#endif

#endif /* CT_CSR_H */
