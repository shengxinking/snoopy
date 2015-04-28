/*
 * SNOOPY LOGGER
 *
 * File: configuration.c
 *
 * Copyright (c) 2014 bostjan@a2o.si
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */



/*
 * Include all required C resources
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <syslog.h>



/*
 * Include all snoopy-related resources
 */
#include "snoopy.h"
#include "configuration.h"



/*
 * Include configfile-related resources
 */
#ifdef SNOOPY_CONFIG_FILE
#include "configfile.h"
#endif



/*
 * Storage of snoopy configuration, with default values
 */
snoopy_configuration_type   snoopy_configuration = {
    .initialized             = SNOOPY_TRUE,

#ifdef SNOOPY_CONFIG_FILE
    .config_file_enabled     = SNOOPY_TRUE,
#else
    .config_file_enabled     = SNOOPY_FALSE,
#endif
    .config_file_path        = "",
    .config_file_found       = SNOOPY_FALSE,
    .config_file_parsed      = SNOOPY_FALSE,

#ifdef SNOOPY_ERROR_LOGGING_ENABLED
    .error_logging_enabled   = SNOOPY_TRUE,
#else
    .error_logging_enabled   = SNOOPY_FALSE,
#endif

    .message_format          = SNOOPY_LOG_MESSAGE_FORMAT,
    .message_format_malloced = SNOOPY_FALSE,

#ifdef SNOOPY_FILTER_ENABLED
    .filter_enabled          = SNOOPY_TRUE,
#else
    .filter_enabled          = SNOOPY_FALSE,
#endif
    .filter_chain            = SNOOPY_FILTER_CHAIN,
    .filter_chain_malloced   = SNOOPY_FALSE,

    .output_provider         = SNOOPY_OUTPUT_PROVIDER,
    .output_path             = SNOOPY_OUTPUT_PATH,

    .syslog_facility         = SNOOPY_CONF_SYSLOG_FACILITY,
    .syslog_level            = SNOOPY_CONF_SYSLOG_LEVEL,
};



/*
 * snoopy_configuration_ctor
 *
 * Description:
 *     Populates snoopy_configuration config variable storage with
 *     correct values, either from configuration file (if enabled)
 *     or from ./configure arguments, or defaults are used as last
 *     case scenario.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_configuration_ctor ()
{
    /* Parse INI file if enabled */
#ifdef SNOOPY_CONFIG_FILE
    snoopy_configfile_load(SNOOPY_CONFIG_FILE);
#endif
}



/*
 * snoopy_configuration_dtor
 *
 * Description:
 *     Frees all configuration-related malloced resources.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_configuration_dtor ()
{
    if (SNOOPY_TRUE == snoopy_configuration.message_format_malloced) {
        free(snoopy_configuration.message_format);
//        snoopy_configuration.message_format = SNOOPY_LOG_MESSAGE_FORMAT;

        /*
         * Set this to false - REQUIRED
         *
         * This needs to be done as a special condition can occur at boot/shutdown:
         * - snoopy is loaded when snoopy.ini is visible (mounted, present)
         * - snoopy parses it, and sets message_format and ..._malloced to TRUE
         * - on shutdown, snoopy.ini might disappear
         * - snoopy_configuration_ctor() tries to parse config file, and as it is
         *     not found, it does no alteration of snoopy_configuraton struct
         * - snoopy_configuration.message_format_malloced is left set to TRUE
         * - when snoopy_configuration_dtor() is called, it tries to free the
         *     const char[] that contains the compiled-in message format
         */
        snoopy_configuration.message_format_malloced = SNOOPY_FALSE;
    }


    if (SNOOPY_TRUE == snoopy_configuration.filter_chain_malloced) {
        free(snoopy_configuration.filter_chain);
//        snoopy_configuration.filter_chain = SNOOPY_FILTER_CHAIN;

        /*
         * Set this to false - REQUIRED
         *
         * See comment above, same principle applies here.
         */
        snoopy_configuration.filter_chain_malloced = SNOOPY_FALSE;
    }
}
