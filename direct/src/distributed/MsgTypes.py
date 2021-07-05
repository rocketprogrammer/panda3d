"""MsgTypes module: contains distributed object message types"""

from direct.showbase.PythonUtil import invertDictLossless
from .DisneyMessageTypes import *

MsgName2Id = {
    # 2 new params: passwd, char bool 0/1 1 = new account
    # 2 new return values: 129 = not found, 12 = bad passwd,
    'CLIENT_LOGIN':                                  1,
    'CLIENT_LOGIN_RESP':                             2,
    'CLIENT_GET_AVATARS':                            3,
    # Sent by the server when it is dropping the connection deliberately.
    'CLIENT_GO_GET_LOST':                            4,
    'CLIENT_GET_AVATARS_RESP':                       5,
    'CLIENT_CREATE_AVATAR':                          6,
    'CLIENT_CREATE_AVATAR_RESP':                     7,
    'CLIENT_GET_SHARD_LIST':                         8,
    'CLIENT_GET_SHARD_LIST_RESP':                    9,
    'CLIENT_GET_FRIEND_LIST':                        10,
    'CLIENT_GET_FRIEND_LIST_RESP':                   11,
    'CLIENT_GET_AVATAR_DETAILS':                     14,
    'CLIENT_GET_AVATAR_DETAILS_RESP':                15,
    'CLIENT_LOGIN_2':                                16,
    'CLIENT_LOGIN_2_RESP':                           17,

    'CLIENT_OBJECT_UPDATE_FIELD':                    24,
    'CLIENT_OBJECT_UPDATE_FIELD_RESP':               24,
    'CLIENT_OBJECT_DISABLE':                         25,
    'CLIENT_OBJECT_DISABLE_RESP':                    25,
    'CLIENT_OBJECT_DISABLE_OWNER':                   26,
    'CLIENT_OBJECT_DISABLE_OWNER_RESP':              26,
    'CLIENT_OBJECT_DELETE':                          27,
    'CLIENT_OBJECT_DELETE_RESP':                     27,
    'CLIENT_SET_ZONE':                               29,
    'CLIENT_SET_ZONE_CMU':                           29,
    'CLIENT_REMOVE_ZONE':                            30,
    'CLIENT_SET_SHARD':                              31,
    'CLIENT_SET_AVATAR':                             32,
    'CLIENT_CREATE_OBJECT_REQUIRED':                 34,
    'CLIENT_CREATE_OBJECT_REQUIRED_RESP':            34,
    'CLIENT_CREATE_OBJECT_REQUIRED_OTHER':           35,
    'CLIENT_CREATE_OBJECT_REQUIRED_OTHER_RESP':      35,
    'CLIENT_CREATE_OBJECT_REQUIRED_OTHER_OWNER':     36,
    'CLIENT_CREATE_OBJECT_REQUIRED_OTHER_OWNER_RESP':36,

    'CLIENT_REQUEST_GENERATES':                      36,

    'CLIENT_DISCONNECT':                             37,

    'CLIENT_GET_STATE_RESP':                         47,
    'CLIENT_DONE_INTEREST_RESP':                     48,

    'CLIENT_DELETE_AVATAR':                          49,

    'CLIENT_DELETE_AVATAR_RESP':                     5,

    'CLIENT_HEARTBEAT':                              52,
    'CLIENT_FRIEND_ONLINE':                          53,
    'CLIENT_FRIEND_OFFLINE':                         54,
    'CLIENT_REMOVE_FRIEND':                          56,

    'CLIENT_CHANGE_PASSWORD':                        65,

    'CLIENT_SET_NAME_PATTERN':                       67,
    'CLIENT_SET_NAME_PATTERN_ANSWER':                68,

    'CLIENT_OBJECT_SET_FIELD':                       120,
    'CLIENT_OBJECT_SET_FIELDS':                      121,
    'CLIENT_OBJECT_LEAVING':                         132,
    'CLIENT_OBJECT_LEAVING_OWNER':                   161,
    'CLIENT_ENTER_OBJECT_REQUIRED':                  142,
    'CLIENT_ENTER_OBJECT_REQUIRED_OTHER':            143,
    'CLIENT_ENTER_OBJECT_REQUIRED_OWNER':            172,
    'CLIENT_ENTER_OBJECT_REQUIRED_OTHER_OWNER':      173,

    'CLIENT_DONE_INTEREST_RESP':                     204,

    'CLIENT_ADD_INTEREST':                           97,
    'CLIENT_ADD_INTEREST_MULTIPLE':                  201,
    'CLIENT_REMOVE_INTEREST':                        99,
    'CLIENT_OBJECT_LOCATION':                        102,

    # These are sent internally inside the Astron cluster.

    # Message Director control messages:
    'CONTROL_CHANNEL':                                  1,
    'CONTROL_ADD_CHANNEL':                              9000,
    'CONTROL_REMOVE_CHANNEL':                           9001,
    'CONTROL_ADD_RANGE':                                9002,
    'CONTROL_REMOVE_RANGE':                             9003,
    'CONTROL_ADD_POST_REMOVE':                          9010,
    'CONTROL_CLEAR_POST_REMOVES':                       9011,

    # State Server control messages:
    'STATESERVER_CREATE_OBJECT_WITH_REQUIRED':          2000,
    'STATESERVER_CREATE_OBJECT_WITH_REQUIRED_OTHER':    2001,
    'STATESERVER_DELETE_AI_OBJECTS':                    2009,
    'STATESERVER_OBJECT_GET_FIELD':                     2010,
    'STATESERVER_OBJECT_GET_FIELD_RESP':                2011,
    'STATESERVER_OBJECT_GET_FIELDS':                    2012,
    'STATESERVER_OBJECT_GET_FIELDS_RESP':               2013,
    'STATESERVER_OBJECT_GET_ALL':                       2014,
    'STATESERVER_OBJECT_GET_ALL_RESP':                  2015,
    'STATESERVER_OBJECT_SET_FIELD':                     2020,
    'STATESERVER_OBJECT_SET_FIELDS':                    2021,
    'STATESERVER_OBJECT_DELETE_FIELD_RAM':              2030,
    'STATESERVER_OBJECT_DELETE_FIELDS_RAM':             2031,
    'STATESERVER_OBJECT_DELETE_RAM':                    2032,
    'STATESERVER_OBJECT_SET_LOCATION':                          2040,
    'STATESERVER_OBJECT_CHANGING_LOCATION':                     2041,
    'STATESERVER_OBJECT_ENTER_LOCATION_WITH_REQUIRED':          2042,
    'STATESERVER_OBJECT_ENTER_LOCATION_WITH_REQUIRED_OTHER':    2043,
    'STATESERVER_OBJECT_GET_LOCATION':                          2044,
    'STATESERVER_OBJECT_GET_LOCATION_RESP':                     2045,
    'STATESERVER_OBJECT_LOCATION_ACK':                          2046,
    'STATESERVER_OBJECT_SET_AI':                                2050,
    'STATESERVER_OBJECT_CHANGING_AI':                           2051,
    'STATESERVER_OBJECT_ENTER_AI_WITH_REQUIRED':                2052,
    'STATESERVER_OBJECT_ENTER_AI_WITH_REQUIRED_OTHER':          2053,
    'STATESERVER_OBJECT_GET_AI':                                2054,
    'STATESERVER_OBJECT_GET_AI_RESP':                           2055,
    'STATESERVER_OBJECT_SET_OWNER':                             2060,
    'STATESERVER_OBJECT_CHANGING_OWNER':                        2061,
    'STATESERVER_OBJECT_ENTER_OWNER_WITH_REQUIRED':             2062,
    'STATESERVER_OBJECT_ENTER_OWNER_WITH_REQUIRED_OTHER':       2063,
    'STATESERVER_OBJECT_GET_OWNER':                             2064,
    'STATESERVER_OBJECT_GET_OWNER_RESP':                        2065,
    'STATESERVER_OBJECT_GET_ZONE_OBJECTS':              2100,
    'STATESERVER_OBJECT_GET_ZONES_OBJECTS':             2102,
    'STATESERVER_OBJECT_GET_CHILDREN':                  2104,
    'STATESERVER_OBJECT_GET_ZONE_COUNT':                2110,
    'STATESERVER_OBJECT_GET_ZONE_COUNT_RESP':           2111,
    'STATESERVER_OBJECT_GET_ZONES_COUNT':               2112,
    'STATESERVER_OBJECT_GET_ZONES_COUNT_RESP':          2113,
    'STATESERVER_OBJECT_GET_CHILD_COUNT':               2114,
    'STATESERVER_OBJECT_GET_CHILD_COUNT_RESP':          2115,
    'STATESERVER_OBJECT_DELETE_ZONE':                   2120,
    'STATESERVER_OBJECT_DELETE_ZONES':                  2122,
    'STATESERVER_OBJECT_DELETE_CHILDREN':               2124,
    # DBSS-backed-object messages:
    'DBSS_OBJECT_ACTIVATE_WITH_DEFAULTS':        2200,
    'DBSS_OBJECT_ACTIVATE_WITH_DEFAULTS_OTHER':  2201,
    'DBSS_OBJECT_GET_ACTIVATED':                 2207,
    'DBSS_OBJECT_GET_ACTIVATED_RESP':            2208,
    'DBSS_OBJECT_DELETE_FIELD_DISK':             2230,
    'DBSS_OBJECT_DELETE_FIELDS_DISK':            2231,
    'DBSS_OBJECT_DELETE_DISK':                   2232,

    # Database Server control messages:
    'DBSERVER_CREATE_OBJECT':                       3000,
    'DBSERVER_CREATE_OBJECT_RESP':                  3001,
    'DBSERVER_OBJECT_GET_FIELD':                    3010,
    'DBSERVER_OBJECT_GET_FIELD_RESP':               3011,
    'DBSERVER_OBJECT_GET_FIELDS':                   3012,
    'DBSERVER_OBJECT_GET_FIELDS_RESP':              3013,
    'DBSERVER_OBJECT_GET_ALL':                      3014,
    'DBSERVER_OBJECT_GET_ALL_RESP':                 3015,
    'DBSERVER_OBJECT_SET_FIELD':                    3020,
    'DBSERVER_OBJECT_SET_FIELDS':                   3021,
    'DBSERVER_OBJECT_SET_FIELD_IF_EQUALS':          3022,
    'DBSERVER_OBJECT_SET_FIELD_IF_EQUALS_RESP':     3023,
    'DBSERVER_OBJECT_SET_FIELDS_IF_EQUALS':         3024,
    'DBSERVER_OBJECT_SET_FIELDS_IF_EQUALS_RESP':    3025,
    'DBSERVER_OBJECT_SET_FIELD_IF_EMPTY':           3026,
    'DBSERVER_OBJECT_SET_FIELD_IF_EMPTY_RESP':      3027,
    'DBSERVER_OBJECT_DELETE_FIELD':                 3030,
    'DBSERVER_OBJECT_DELETE_FIELDS':                3031,
    'DBSERVER_OBJECT_DELETE':                       3032,

    # Client Agent control messages:
    'CLIENTAGENT_SET_STATE':                        1000,
    'CLIENTAGENT_SET_CLIENT_ID':                    1001,
    'CLIENTAGENT_SEND_DATAGRAM':                    1002,
    'CLIENTAGENT_EJECT':                            1004,
    'CLIENTAGENT_DROP':                             1005,
    'CLIENTAGENT_GET_NETWORK_ADDRESS':              1006,
    'CLIENTAGENT_GET_NETWORK_ADDRESS_RESP':         1007,
    'CLIENTAGENT_DECLARE_OBJECT':                   1010,
    'CLIENTAGENT_UNDECLARE_OBJECT':                 1011,
    'CLIENTAGENT_ADD_SESSION_OBJECT':               1012,
    'CLIENTAGENT_REMOVE_SESSION_OBJECT':            1013,
    'CLIENTAGENT_SET_FIELDS_SENDABLE':              1014,
    'CLIENTAGENT_OPEN_CHANNEL':                     1100,
    'CLIENTAGENT_CLOSE_CHANNEL':                    1101,
    'CLIENTAGENT_ADD_POST_REMOVE':                  1110,
    'CLIENTAGENT_CLEAR_POST_REMOVES':               1111,
    'CLIENTAGENT_ADD_INTEREST':                     1200,
    'CLIENTAGENT_ADD_INTEREST_MULTIPLE':            1201,
    'CLIENTAGENT_REMOVE_INTEREST':                  1203,
    'CLIENTAGENT_DONE_INTEREST_RESP':               1204,
    }

# create id->name table for debugging
MsgId2Names = invertDictLossless(MsgName2Id)

# put msg names in module scope, assigned to msg value
for name, value in MsgName2Id.items():
    exec('%s = %s' % (name, value))
del name, value

# These messages are ignored when the client is headed to the quiet zone
QUIET_ZONE_IGNORED_LIST = [

    # We mustn't ignore updates, because some updates for localToon
    # are always important.
    #CLIENT_OBJECT_UPDATE_FIELD,

    # These are now handled. If it is a create for a class that is in the
    # uber zone, we should create it.
    #CLIENT_CREATE_OBJECT_REQUIRED,
    #CLIENT_CREATE_OBJECT_REQUIRED_OTHER,

    ]

# The following is a different set of numbers from above.
# These are the sub-message types for CLIENT_LOGIN_2.
CLIENT_LOGIN_2_GREEN = 1       # Disney's GoReg subscription token, not used.
CLIENT_LOGIN_2_PLAY_TOKEN = 2  # VR Studio PlayToken.
CLIENT_LOGIN_2_BLUE = 3        # The international GoReg token.
CLIENT_LOGIN_3_DISL_TOKEN = 4  # SSL encoded blob from DISL system.

# ExtAgent messages.
CLIENTAGENT_EXTAGENT_MESSAGE = 1205
CLIENTAGENT_EXTAGENT_RESPONSE = 1206
