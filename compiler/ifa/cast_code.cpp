switch (to->const_kind) {
 default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
 case NUM_KIND_UINT: 
   switch (to->num_index) {
   case INT_SIZE_1:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_bool = (bool)!!from->v_bool; break;
       case INT_SIZE_8:
         to->v_bool = (bool)!!from->v_uint8; break;
       case INT_SIZE_16:
         to->v_bool = (bool)!!from->v_uint16; break;
       case INT_SIZE_32:
         to->v_bool = (bool)!!from->v_uint32; break;
       case INT_SIZE_64:
         to->v_bool = (bool)!!from->uint_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_bool = (bool)!!from->v_bool; break;
       case INT_SIZE_8:
         to->v_bool = (bool)!!from->v_int8; break;
       case INT_SIZE_16:
         to->v_bool = (bool)!!from->v_int16; break;
       case INT_SIZE_32:
         to->v_bool = (bool)!!from->v_int32; break;
       case INT_SIZE_64:
         to->v_bool = (bool)!!from->int_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_FLOAT:
       switch (from->num_index) {
       case FLOAT_SIZE_16:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_32:
         to->v_bool = (bool)!!from->v_float32; break;
       case FLOAT_SIZE_48:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_64:
         to->v_bool = (bool)!!from->v_float64; break;
       case FLOAT_SIZE_80:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_96:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_112:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_128:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case INT_SIZE_8:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_uint8 = (uint8)from->v_bool; break;
       case INT_SIZE_8:
         to->v_uint8 = (uint8)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_uint8 = (uint8)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_uint8 = (uint8)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_uint8 = (uint8)from->uint_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_uint8 = (uint8)from->v_bool; break;
       case INT_SIZE_8:
         to->v_uint8 = (uint8)from->v_int8; break;
       case INT_SIZE_16:
         to->v_uint8 = (uint8)from->v_int16; break;
       case INT_SIZE_32:
         to->v_uint8 = (uint8)from->v_int32; break;
       case INT_SIZE_64:
         to->v_uint8 = (uint8)from->int_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_FLOAT:
       switch (from->num_index) {
       case FLOAT_SIZE_16:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_32:
         to->v_uint8 = (uint8)from->v_float32; break;
       case FLOAT_SIZE_48:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_64:
         to->v_uint8 = (uint8)from->v_float64; break;
       case FLOAT_SIZE_80:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_96:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_112:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_128:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case INT_SIZE_16:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_uint16 = (uint16)from->v_bool; break;
       case INT_SIZE_8:
         to->v_uint16 = (uint16)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_uint16 = (uint16)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_uint16 = (uint16)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_uint16 = (uint16)from->uint_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_uint16 = (uint16)from->v_bool; break;
       case INT_SIZE_8:
         to->v_uint16 = (uint16)from->v_int8; break;
       case INT_SIZE_16:
         to->v_uint16 = (uint16)from->v_int16; break;
       case INT_SIZE_32:
         to->v_uint16 = (uint16)from->v_int32; break;
       case INT_SIZE_64:
         to->v_uint16 = (uint16)from->int_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_FLOAT:
       switch (from->num_index) {
       case FLOAT_SIZE_16:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_32:
         to->v_uint16 = (uint16)from->v_float32; break;
       case FLOAT_SIZE_48:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_64:
         to->v_uint16 = (uint16)from->v_float64; break;
       case FLOAT_SIZE_80:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_96:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_112:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_128:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case INT_SIZE_32:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_uint32 = (uint32)from->v_bool; break;
       case INT_SIZE_8:
         to->v_uint32 = (uint32)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_uint32 = (uint32)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_uint32 = (uint32)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_uint32 = (uint32)from->uint_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_uint32 = (uint32)from->v_bool; break;
       case INT_SIZE_8:
         to->v_uint32 = (uint32)from->v_int8; break;
       case INT_SIZE_16:
         to->v_uint32 = (uint32)from->v_int16; break;
       case INT_SIZE_32:
         to->v_uint32 = (uint32)from->v_int32; break;
       case INT_SIZE_64:
         to->v_uint32 = (uint32)from->int_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_FLOAT:
       switch (from->num_index) {
       case FLOAT_SIZE_16:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_32:
         to->v_uint32 = (uint32)from->v_float32; break;
       case FLOAT_SIZE_48:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_64:
         to->v_uint32 = (uint32)from->v_float64; break;
       case FLOAT_SIZE_80:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_96:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_112:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_128:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case INT_SIZE_64:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_uint64 = (uint64)from->v_bool; break;
       case INT_SIZE_8:
         to->v_uint64 = (uint64)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_uint64 = (uint64)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_uint64 = (uint64)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_uint64 = (uint64)from->uint_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_uint64 = (uint64)from->v_bool; break;
       case INT_SIZE_8:
         to->v_uint64 = (uint64)from->v_int8; break;
       case INT_SIZE_16:
         to->v_uint64 = (uint64)from->v_int16; break;
       case INT_SIZE_32:
         to->v_uint64 = (uint64)from->v_int32; break;
       case INT_SIZE_64:
         to->v_uint64 = (uint64)from->int_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_FLOAT:
       switch (from->num_index) {
       case FLOAT_SIZE_16:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_32:
         to->v_uint64 = (uint64)from->v_float32; break;
       case FLOAT_SIZE_48:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_64:
         to->v_uint64 = (uint64)from->v_float64; break;
       case FLOAT_SIZE_80:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_96:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_112:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_128:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   default:
     INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
   } break;
 case NUM_KIND_INT: 
   switch (to->num_index) {
   case INT_SIZE_1:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_bool = (bool)from->v_bool; break;
       case INT_SIZE_8:
         to->v_bool = (bool)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_bool = (bool)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_bool = (bool)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_bool = (bool)from->uint_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_bool = (bool)from->v_bool; break;
       case INT_SIZE_8:
         to->v_bool = (bool)from->v_int8; break;
       case INT_SIZE_16:
         to->v_bool = (bool)from->v_int16; break;
       case INT_SIZE_32:
         to->v_bool = (bool)from->v_int32; break;
       case INT_SIZE_64:
         to->v_bool = (bool)from->int_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_FLOAT:
       switch (from->num_index) {
       case FLOAT_SIZE_16:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_32:
         to->v_bool = (bool)from->v_float32; break;
       case FLOAT_SIZE_48:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_64:
         to->v_bool = (bool)from->v_float64; break;
       case FLOAT_SIZE_80:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_96:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_112:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_128:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case INT_SIZE_8:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_int8 = (int8)from->v_bool; break;
       case INT_SIZE_8:
         to->v_int8 = (int8)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_int8 = (int8)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_int8 = (int8)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_int8 = (int8)from->uint_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_int8 = (int8)from->v_bool; break;
       case INT_SIZE_8:
         to->v_int8 = (int8)from->v_int8; break;
       case INT_SIZE_16:
         to->v_int8 = (int8)from->v_int16; break;
       case INT_SIZE_32:
         to->v_int8 = (int8)from->v_int32; break;
       case INT_SIZE_64:
         to->v_int8 = (int8)from->int_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_FLOAT:
       switch (from->num_index) {
       case FLOAT_SIZE_16:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_32:
         to->v_int8 = (int8)from->v_float32; break;
       case FLOAT_SIZE_48:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_64:
         to->v_int8 = (int8)from->v_float64; break;
       case FLOAT_SIZE_80:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_96:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_112:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_128:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case INT_SIZE_16:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_int16 = (int16)from->v_bool; break;
       case INT_SIZE_8:
         to->v_int16 = (int16)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_int16 = (int16)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_int16 = (int16)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_int16 = (int16)from->uint_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_int16 = (int16)from->v_bool; break;
       case INT_SIZE_8:
         to->v_int16 = (int16)from->v_int8; break;
       case INT_SIZE_16:
         to->v_int16 = (int16)from->v_int16; break;
       case INT_SIZE_32:
         to->v_int16 = (int16)from->v_int32; break;
       case INT_SIZE_64:
         to->v_int16 = (int16)from->int_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_FLOAT:
       switch (from->num_index) {
       case FLOAT_SIZE_16:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_32:
         to->v_int16 = (int16)from->v_float32; break;
       case FLOAT_SIZE_48:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_64:
         to->v_int16 = (int16)from->v_float64; break;
       case FLOAT_SIZE_80:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_96:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_112:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_128:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case INT_SIZE_32:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_int32 = (int32)from->v_bool; break;
       case INT_SIZE_8:
         to->v_int32 = (int32)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_int32 = (int32)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_int32 = (int32)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_int32 = (int32)from->uint_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_int32 = (int32)from->v_bool; break;
       case INT_SIZE_8:
         to->v_int32 = (int32)from->v_int8; break;
       case INT_SIZE_16:
         to->v_int32 = (int32)from->v_int16; break;
       case INT_SIZE_32:
         to->v_int32 = (int32)from->v_int32; break;
       case INT_SIZE_64:
         to->v_int32 = (int32)from->int_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_FLOAT:
       switch (from->num_index) {
       case FLOAT_SIZE_16:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_32:
         to->v_int32 = (int32)from->v_float32; break;
       case FLOAT_SIZE_48:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_64:
         to->v_int32 = (int32)from->v_float64; break;
       case FLOAT_SIZE_80:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_96:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_112:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_128:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case INT_SIZE_64:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_int64 = (int64)from->v_bool; break;
       case INT_SIZE_8:
         to->v_int64 = (int64)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_int64 = (int64)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_int64 = (int64)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_int64 = (int64)from->uint_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_int64 = (int64)from->v_bool; break;
       case INT_SIZE_8:
         to->v_int64 = (int64)from->v_int8; break;
       case INT_SIZE_16:
         to->v_int64 = (int64)from->v_int16; break;
       case INT_SIZE_32:
         to->v_int64 = (int64)from->v_int32; break;
       case INT_SIZE_64:
         to->v_int64 = (int64)from->int_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_FLOAT:
       switch (from->num_index) {
       case FLOAT_SIZE_16:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_32:
         to->v_int64 = (int64)from->v_float32; break;
       case FLOAT_SIZE_48:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_64:
         to->v_int64 = (int64)from->v_float64; break;
       case FLOAT_SIZE_80:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_96:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_112:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_128:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   default:
     INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
   } break;
 case NUM_KIND_FLOAT:
   switch (to->num_index) {
   case FLOAT_SIZE_16:
     INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
   case FLOAT_SIZE_32:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_float32 = (float32)from->v_bool; break;
       case INT_SIZE_8:
         to->v_float32 = (float32)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_float32 = (float32)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_float32 = (float32)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_float32 = (float32)from->uint_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_float32 = (float32)from->v_bool; break;
       case INT_SIZE_8:
         to->v_float32 = (float32)from->v_int8; break;
       case INT_SIZE_16:
         to->v_float32 = (float32)from->v_int16; break;
       case INT_SIZE_32:
         to->v_float32 = (float32)from->v_int32; break;
       case INT_SIZE_64:
         to->v_float32 = (float32)from->int_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_FLOAT:
       switch (from->num_index) {
       case FLOAT_SIZE_16:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_32:
         to->v_float32 = (float32)from->v_float32; break;
       case FLOAT_SIZE_48:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_64:
         to->v_float32 = (float32)from->v_float64; break;
       case FLOAT_SIZE_80:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_96:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_112:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_128:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case FLOAT_SIZE_48:
     INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
   case FLOAT_SIZE_64:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_float64 = (float64)from->v_bool; break;
       case INT_SIZE_8:
         to->v_float64 = (float64)from->v_uint8; break;
       case INT_SIZE_16:
         to->v_float64 = (float64)from->v_uint16; break;
       case INT_SIZE_32:
         to->v_float64 = (float64)from->v_uint32; break;
       case INT_SIZE_64:
         to->v_float64 = (float64)from->uint_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_1:
         to->v_float64 = (float64)from->v_bool; break;
       case INT_SIZE_8:
         to->v_float64 = (float64)from->v_int8; break;
       case INT_SIZE_16:
         to->v_float64 = (float64)from->v_int16; break;
       case INT_SIZE_32:
         to->v_float64 = (float64)from->v_int32; break;
       case INT_SIZE_64:
         to->v_float64 = (float64)from->int_value(); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_FLOAT:
       switch (from->num_index) {
       case FLOAT_SIZE_16:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_32:
         to->v_float64 = (float64)from->v_float32; break;
       case FLOAT_SIZE_48:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_64:
         to->v_float64 = (float64)from->v_float64; break;
       case FLOAT_SIZE_80:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_96:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_112:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_128:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   case FLOAT_SIZE_80:
     INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
   case FLOAT_SIZE_96:
     INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
   case FLOAT_SIZE_112:
     INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
   case FLOAT_SIZE_128:
     switch (from->const_kind) {
     default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case NUM_KIND_UINT:
       switch (from->num_index) {
       case INT_SIZE_1:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case INT_SIZE_8:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case INT_SIZE_16:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case INT_SIZE_32:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case INT_SIZE_64:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_INT:
       switch (from->num_index) {
       case INT_SIZE_1:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case INT_SIZE_8:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case INT_SIZE_16:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case INT_SIZE_32:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case INT_SIZE_64:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     case NUM_KIND_FLOAT:
       switch (from->num_index) {
       case FLOAT_SIZE_16:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_32:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_48:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_64:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_80:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_96:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_112:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       case FLOAT_SIZE_128:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       default:
         INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       } break;
     } break;
   } break;
 case CONST_KIND_STRING:
   switch (from->const_kind) {
   default: INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
   case NUM_KIND_UINT:
     switch (from->num_index) {
     case INT_SIZE_1:
       to->v_string = from->v_bool ? astr("true") : astr("false"); break;
     case INT_SIZE_8:
       to->v_string = istr(from->v_uint8); break;
     case INT_SIZE_16:
       to->v_string = istr(from->v_uint16); break;
     case INT_SIZE_32:
       to->v_string = istr(from->v_uint32); break;
     case INT_SIZE_64:
       to->v_string = istr(from->uint_value()); break;
     default:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     } break;
   case NUM_KIND_INT:
     switch (from->num_index) {
     case INT_SIZE_1:
       to->v_string = from->v_bool ? astr("true") : astr("false"); break;
     case INT_SIZE_8:
       to->v_string = istr(from->v_int8); break;
     case INT_SIZE_16:
       to->v_string = istr(from->v_int16); break;
     case INT_SIZE_32:
       to->v_string = istr(from->v_int32); break;
     case INT_SIZE_64:
       to->v_string = istr(from->int_value()); break;
     default:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     } break;
   case NUM_KIND_FLOAT:
     switch (from->num_index) {
     case FLOAT_SIZE_16:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case FLOAT_SIZE_32:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       //       to->v_string = istr(from->v_float32); break;
     case FLOAT_SIZE_48:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case FLOAT_SIZE_64:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
       //       to->v_string = istr(from->v_float64); break;
     case FLOAT_SIZE_80:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case FLOAT_SIZE_96:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case FLOAT_SIZE_112:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     case FLOAT_SIZE_128:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     default:
       INT_FATAL("Illegal case in coerce_immediate switch statement"); break;
     } break;
   } break;
 }
