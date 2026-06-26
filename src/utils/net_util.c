#include "net_util.h"
#include "cJSON.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper: get address type string
static const char * get_address_type_string(int type)
{
    switch(type) {
        case IPv6_TYPE_GLOBAL: return "Global";
        case IPv6_TYPE_UNIQUE_LOCAL: return "UniqueLocal";
        case IPv6_TYPE_LINK_LOCAL: return "LinkLocal";
        case IPv6_TYPE_LOOPBACK: return "Loopback";
        case IPv6_TYPE_UNSPECIFIED: return "Unspecified";
        case IPv6_TYPE_MULTICAST: return "Multicast";
        default: return "Unknown";
    }
}

static const char * get_state_string(int state)
{
    switch(state) {
        case IPv6_STATE_PREFERRED: return "Preferred";
        case IPv6_STATE_DEPRECATED: return "Deprecated";
        case IPv6_STATE_INVALID: return "Invalid";
        default: return "Unknown";
    }
}

static int get_state_value(const char * state_str)
{
    if(state_str == NULL) return IPv6_STATE_UNKNOWN;

    if(strcasecmp(state_str, "Preferred") == 0) return IPv6_STATE_PREFERRED;
    if(strcasecmp(state_str, "Deprecated") == 0) return IPv6_STATE_DEPRECATED;
    if(strcasecmp(state_str, "Invalid") == 0) return IPv6_STATE_INVALID;

    return IPv6_STATE_UNKNOWN;
}

static int get_address_type_from_string(const char * type_str)
{
    if(type_str == NULL) return IPv6_TYPE_UNKNOWN;

    if(strcasecmp(type_str, "Global") == 0) return IPv6_TYPE_GLOBAL;
    if(strcasecmp(type_str, "UniqueLocal") == 0) return IPv6_TYPE_UNIQUE_LOCAL;
    if(strcasecmp(type_str, "LinkLocal") == 0) return IPv6_TYPE_LINK_LOCAL;
    if(strcasecmp(type_str, "Loopback") == 0) return IPv6_TYPE_LOOPBACK;
    if(strcasecmp(type_str, "Unspecified") == 0) return IPv6_TYPE_UNSPECIFIED;
    if(strcasecmp(type_str, "Multicast") == 0) return IPv6_TYPE_MULTICAST;

    return IPv6_TYPE_UNKNOWN;
}

char * insert_newline_at_position(const char * str, int pos)
{
    if(!str || pos < 0 || pos > strlen(str)) {
        return NULL;
    }

    // Allocate: original length + newline (1) + null terminator (1)
    char * result = malloc(strlen(str) + 2);
    if(!result) {
        return NULL;
    }

    // Copy first part
    strncpy(result, str, pos);
    result[pos] = '\0';

    // Append newline
    strcat(result, "\n");

    // Append second part
    strcat(result, str + pos);

    return result;
}

// Expand abbreviated IPv6 notation
char * expand_ipv6_address(const char * address)
{
    if(!address) return NULL;

    char * result = malloc(40);
    if(!result) return NULL;
    result[0] = '\0';

    char input[40];
    strncpy(input, address, sizeof(input) - 1);
    input[sizeof(input) - 1] = '\0';

    // Full form (7 colons): pad groups with leading zeros
    int colon_count = 0;
    for(int i = 0; input[i] != '\0'; i++) {
        if(input[i] == ':') colon_count++;
    }

    if(colon_count == 7) {
        // Full IPv6 address: pad each group to 4 hex digits
        char *groups[8];
        int group_count = 0;
        
        char *token = strtok(input, ":");
        while(token != NULL && group_count < 8) {
            groups[group_count++] = token;
            token = strtok(NULL, ":");
        }
        
        for(int i = 0; i < 8; i++) {
            if(i > 0) strcat(result, ":");
            
            if(i < group_count) {
                int len = strlen(groups[i]);
                if(len < 4) {
                    // Pad leading zeros
                    for(int j = 0; j < 4 - len; j++) {
                        strcat(result, "0");
                    }
                }
                strcat(result, groups[i]);
            } else {
                strcat(result, "0000");
            }
        }
    } else {
        // Handle "::" compression
        char expanded[40] = {0};
        
        // Expand "::"
        int double_colon_pos = -1;
        int len = strlen(input);
        for(int i = 0; i < len - 1; i++) {
            if(input[i] == ':' && input[i + 1] == ':') {
                double_colon_pos = i;
                break;
            }
        }
        
        if(double_colon_pos != -1) {
            // Count groups on left and right of "::"
            int left_groups = 0;
            int right_groups = 0;
            
            char *ptr = input;
            char *double_colon = input + double_colon_pos;
            
            // Count left-side groups
            char *p = input;
            while(p < double_colon) {
                if(*p == ':') {
                    left_groups++;
                    p++;
                } else {
                    // Skip current character
                    while(p < double_colon && *p != ':') p++;
                }
            }
            if(double_colon_pos > 0 && input[double_colon_pos-1] != ':') {
                left_groups++;
            }
            
            // Count right-side groups
            p = input + double_colon_pos + 2;
            if(*p != '\0') {
                right_groups = 1;
                for(; *p != '\0'; p++) {
                    if(*p == ':') right_groups++;
                }
            }
            
            // Number of zero groups to insert
            int groups_needed = 8 - (left_groups + right_groups);
            if(groups_needed < 0) groups_needed = 0;
            
            // Build expanded string
            int pos = 0;
            
            // Copy left part
            for(int i = 0; i < double_colon_pos; i++) {
                expanded[pos++] = input[i];
            }
            if(double_colon_pos > 0) {
                expanded[pos++] = ':';
            }
            
            // Insert zero groups
            for(int i = 0; i < groups_needed; i++) {
                if(i > 0) expanded[pos++] = ':';
                expanded[pos++] = '0';
            }
            
            // Copy right part
            char *right = input + double_colon_pos + 2;
            if(*right != '\0') {
                if(pos > 0 && expanded[pos-1] != ':') {
                    expanded[pos++] = ':';
                }
                while(*right != '\0') {
                    expanded[pos++] = *right++;
                }
            }
            expanded[pos] = '\0';
        } else {
            strcpy(expanded, input);
        }
        
        // Expand each group to 4 hex digits
        char *groups[8];
        int group_count = 0;
        char expanded_copy[40];
        strcpy(expanded_copy, expanded);
        
        char *token = strtok(expanded_copy, ":");
        while(token != NULL && group_count < 8) {
            groups[group_count++] = token;
            token = strtok(NULL, ":");
        }
        
        for(int i = 0; i < 8; i++) {
            if(i > 0) strcat(result, ":");
            
            if(i < group_count) {
                int len = strlen(groups[i]);
                if(len < 4) {
                    for(int j = 0; j < 4 - len; j++) {
                        strcat(result, "0");
                    }
                }
                strcat(result, groups[i]);
            } else {
                strcat(result, "0000");
            }
        }
    }
    
    return result;
}

// Select IPv6 address to display
static char * get_show_ipv6(IPv6AddressInfo * addrs, int count)
{
    if(count == 0) {
        return NULL;
    }

    // ULA
    for(int i = 0; i < count; i++) {
        if(addrs[i].address_type == IPv6_TYPE_UNIQUE_LOCAL && addrs[i].is_stable &&
           addrs[i].state == IPv6_STATE_PREFERRED) {
            return addrs[i].address;
        }
    }

    for(int i = 0; i < count; i++) {
        if(addrs[i].address_type == IPv6_TYPE_UNIQUE_LOCAL && !addrs[i].is_stable &&
           addrs[i].state == IPv6_STATE_PREFERRED) {
            return addrs[i].address;
        }
    }

    for(int i = 0; i < count; i++) {
        if(addrs[i].address_type == IPv6_TYPE_UNIQUE_LOCAL && addrs[i].is_stable &&
           addrs[i].state == IPv6_STATE_DEPRECATED) {
            return addrs[i].address;
        }
    }

    for(int i = 0; i < count; i++) {
        if(addrs[i].address_type == IPv6_TYPE_UNIQUE_LOCAL && !addrs[i].is_stable &&
           addrs[i].state == IPv6_STATE_DEPRECATED) {
            return addrs[i].address;
        }
    }

    // global
    for(int i = 0; i < count; i++) {
        if(addrs[i].address_type == IPv6_TYPE_GLOBAL && addrs[i].is_stable && addrs[i].state == IPv6_STATE_PREFERRED) {
            return addrs[i].address;
        }
    }

    for(int i = 0; i < count; i++) {
        if(addrs[i].address_type == IPv6_TYPE_GLOBAL && !addrs[i].is_stable && addrs[i].state == IPv6_STATE_PREFERRED) {
            return addrs[i].address;
        }
    }

    return NULL;
}

char * handle_ipv6_report(const char * json_str)
{
    cJSON * root = cJSON_Parse(json_str);
    if(root == NULL) {
        const char * error_ptr = cJSON_GetErrorPtr();
        if(error_ptr != NULL) {
            DEBUG_LOG("JSON parse error before: %s\n", error_ptr);
        }
        // snprintf(response, resp_len, "ERROR=Invalid JSON format");
        return NULL;
    }

    if(!cJSON_IsArray(root)) {
        // snprintf(response, resp_len, "ERROR=JSON is not an array");
        cJSON_Delete(root);
        return NULL;
    }

    int size = cJSON_GetArraySize(root);
    DEBUG_LOG("Found %d IPv6 addresses in JSON\n", size);

    // Allocate array for parsed results
    IPv6AddressInfo * addrs = NULL;
    if(size > 0) {
        addrs = (IPv6AddressInfo *)malloc(size * sizeof(IPv6AddressInfo));
        if(!addrs) {
            // snprintf(response, resp_len, "ERROR=Memory allocation failed");
            cJSON_Delete(root);
            return NULL;
        }
    }
    int index = 0;

    for(int i = 0; i < size; i++) {
        cJSON * item = cJSON_GetArrayItem(root, i);
        if(item != NULL && cJSON_IsObject(item)) {
            int address_type = IPv6_TYPE_UNKNOWN;
            int state        = IPv6_STATE_UNKNOWN;

            cJSON * address_type_item = cJSON_GetObjectItem(item, "address_type");
            if(address_type_item != NULL && cJSON_IsString(address_type_item)) {
                address_type           = get_address_type_from_string(address_type_item->valuestring);
                bool is_supported_type = (address_type == IPv6_TYPE_GLOBAL || address_type == IPv6_TYPE_UNIQUE_LOCAL);
                if(!is_supported_type) {
                    continue;
                }
            } else {
                continue;
            }

            cJSON * state_item = cJSON_GetObjectItem(item, "state");
            if(state_item != NULL && cJSON_IsString(state_item)) {

                state = get_state_value(state_item->valuestring);
                if(state == 2) {
                    continue;
                }
            }

            // Initialize struct
            memset(&addrs[index], 0, sizeof(IPv6AddressInfo));
            addrs[index].address_type = address_type;
            addrs[index].state        = state;
            addrs[index].is_stable    = false;
            // addrs[i].valid_lifetime     = -1;
            // addrs[i].preferred_lifetime = -1;

            cJSON * address_item = cJSON_GetObjectItem(item, "address");
            if(address_item != NULL && cJSON_IsString(address_item)) {
                strncpy(addrs[index].address, address_item->valuestring, sizeof(addrs[index].address) - 1);
            }

            // cJSON * scope_item = cJSON_GetObjectItem(item, "scope");
            // if(scope_item != NULL && cJSON_IsNumber(scope_item)) {
            //     addrs[i].scope = scope_item->valueint;
            // }

            cJSON * is_stable_item = cJSON_GetObjectItem(item, "is_stable");
            if(is_stable_item != NULL && cJSON_IsBool(is_stable_item)) {
                addrs[index].is_stable = cJSON_IsTrue(is_stable_item);
            }

            index++;
        } else {
            DEBUG_LOG("Warning: Item %d is not a valid JSON object\n", i);
        }
    }
    DEBUG_LOG("Parsed %d valid IPv6 addresses\n", index);

    for(int i = 0; i < index; i++) {
        DEBUG_LOG("Address: %s, Type: %s, State: %s, Stable: %s\n",
            addrs[i].address,
            get_address_type_string(addrs[i].address_type),
            get_state_string(addrs[i].state),
            addrs[i].is_stable ? "true" : "false");
    }

    // Select IPv6 address to display
    char * selected_ip = get_show_ipv6(addrs, index);
    if(selected_ip) {
        selected_ip = expand_ipv6_address(selected_ip);
        // selected_ip = insert_newline_at_position(selected_ip, 35);
    }
    if(addrs) {
        free(addrs);
    }
    cJSON_Delete(root);

    return selected_ip;
}