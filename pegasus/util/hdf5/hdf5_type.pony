
//================================================================
// file types
//================================================================

type HidType is I64 // file id
type HerrType is I32 // error type

type H5FScopeType is
    ( H5FSCopeLocal
    | H5FScopeGlobal
    )

type H5FFileFlag is
    ( H5FAccRDOnly
    | H5FAccRDWR
    | H5FAccTRUNC
    | H5FAccEXCL
    )

primitive H5FAccRDOnly      fun apply(): U32 => 0
primitive H5FAccRDWR        fun apply(): U32 => 1
primitive H5FAccTRUNC       fun apply(): U32 => 2
primitive H5FAccEXCL        fun apply(): U32 => 4

primitive H5FSCopeLocal     fun apply(): I32 => 0
primitive H5FScopeGlobal    fun apply(): I32 => 1

primitive H5PDefault        fun apply(): I64 => 0

//================================================================
// table types
//================================================================
