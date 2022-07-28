# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`struct `[`bNode`](#structbNode) | Struct that represents a B-Tree node. The node type has 3 possible values: ROOT(0), MID(1) and LEAF(2)
`struct `[`bTree`](#structbTree) | Struct that represents a B-Tree.
`struct `[`index_data_s1`](#structindex__data__s1) | Struct that represents a single item data in the index.
`struct `[`index_data_s2`](#structindex__data__s2) | Struct that represents a single item data in the index.
`struct `[`index_s1`](#structindex__s1) | Struct that stores a array of index_data_st1 and it size.
`struct `[`index_s2`](#structindex__s2) | Struct that stores a array of index_data_st1 and it size.
`struct `[`key`](#structkey) | Struct that represents a B-Tree key. Every key has a id, file type and a rrn or byteoffset. A union "value" is used to store different type values on the key.
`struct `[`registry_data_s`](#structregistry__data__s) | Struct that stores registry data.
`struct `[`WField`](#structWField) | Struct that stores searching data.

# struct `bNode` 

Struct that represents a B-Tree node. The node type has 3 possible values: ROOT(0), MID(1) and LEAF(2)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public char `[`nodeType`](#structbNode_1af6c706eed7adb9793fd5504dc43b702f) | 
`public int `[`numKeys`](#structbNode_1ab8e394254c78ff2a5ea6375b311d43b2) | 
`public `[`Key`](#btree_8h_1a2356fb7b976daa7003f1e629351af5ce)` `[`key`](#structbNode_1ac410e83303c004e8277a1b9322612fec) | 
`public int `[`desc`](#structbNode_1ac99e22587bcde2d92813a5caacb9fe75) | 
`public int `[`rrn`](#structbNode_1ad0269da840ae98e2e200183491636000) | 

## Members

#### `public char `[`nodeType`](#structbNode_1af6c706eed7adb9793fd5504dc43b702f) 

#### `public int `[`numKeys`](#structbNode_1ab8e394254c78ff2a5ea6375b311d43b2) 

#### `public `[`Key`](#btree_8h_1a2356fb7b976daa7003f1e629351af5ce)` `[`key`](#structbNode_1ac410e83303c004e8277a1b9322612fec) 

#### `public int `[`desc`](#structbNode_1ac99e22587bcde2d92813a5caacb9fe75) 

#### `public int `[`rrn`](#structbNode_1ad0269da840ae98e2e200183491636000) 

# struct `bTree` 

Struct that represents a B-Tree.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public char `[`status`](#structbTree_1ae317ceae04986fe75798f518ca84b3d5) | 
`public struct `[`bNode`](#structbNode)` * `[`root`](#structbTree_1a8d4acc88f97738169f52841d25ca9186) | 
`public int `[`rootRRN`](#structbTree_1ae7dd528be51e2fc4efa88679a05e3d36) | 
`public int `[`nextRRN`](#structbTree_1a6098952e960a3d0f5342aa1caaebe403) | 
`public int `[`numNodes`](#structbTree_1ab7e931cdb194168712d6139ca4e21d8b) | 

## Members

#### `public char `[`status`](#structbTree_1ae317ceae04986fe75798f518ca84b3d5) 

#### `public struct `[`bNode`](#structbNode)` * `[`root`](#structbTree_1a8d4acc88f97738169f52841d25ca9186) 

#### `public int `[`rootRRN`](#structbTree_1ae7dd528be51e2fc4efa88679a05e3d36) 

#### `public int `[`nextRRN`](#structbTree_1a6098952e960a3d0f5342aa1caaebe403) 

#### `public int `[`numNodes`](#structbTree_1ab7e931cdb194168712d6139ca4e21d8b) 

# struct `index_data_s1` 

Struct that represents a single item data in the index.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int `[`id`](#structindex__data__s1_1a4fa17ecc198474bd2c3ebfc0af893cd4) | 
`public int `[`rrn`](#structindex__data__s1_1a45c767b4d13d41d0534c5af58f429263) | 

## Members

#### `public int `[`id`](#structindex__data__s1_1a4fa17ecc198474bd2c3ebfc0af893cd4) 

#### `public int `[`rrn`](#structindex__data__s1_1a45c767b4d13d41d0534c5af58f429263) 

# struct `index_data_s2` 

Struct that represents a single item data in the index.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int `[`id`](#structindex__data__s2_1a79de52750b96df5cf91bd38b37b1c325) | 
`public long long int `[`byteOffset`](#structindex__data__s2_1ab2c0f1e05483fa4fcb9921d39e616225) | 

## Members

#### `public int `[`id`](#structindex__data__s2_1a79de52750b96df5cf91bd38b37b1c325) 

#### `public long long int `[`byteOffset`](#structindex__data__s2_1ab2c0f1e05483fa4fcb9921d39e616225) 

# struct `index_s1` 

Struct that stores a array of index_data_st1 and it size.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`index_data_st1`](#index__t1_8h_1aa0c440541f5ac7c16f6ebbc28a04dd6d)` * `[`data`](#structindex__s1_1ab0199acdfcfae9f6fc2d867e88219e7d) | 
`public int `[`size`](#structindex__s1_1abbfc10d0b5026e2accdb53a829dcfbaf) | 

## Members

#### `public `[`index_data_st1`](#index__t1_8h_1aa0c440541f5ac7c16f6ebbc28a04dd6d)` * `[`data`](#structindex__s1_1ab0199acdfcfae9f6fc2d867e88219e7d) 

#### `public int `[`size`](#structindex__s1_1abbfc10d0b5026e2accdb53a829dcfbaf) 

# struct `index_s2` 

Struct that stores a array of index_data_st1 and it size.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`index_data_st2`](#index__t2_8h_1a58de49139a4f01bcef70dcebd625c6e0)` * `[`data`](#structindex__s2_1a77becc8883b80d749808b74673f78d9b) | 
`public int `[`size`](#structindex__s2_1a240d921e8475796c81141e21d76755b1) | 

## Members

#### `public `[`index_data_st2`](#index__t2_8h_1a58de49139a4f01bcef70dcebd625c6e0)` * `[`data`](#structindex__s2_1a77becc8883b80d749808b74673f78d9b) 

#### `public int `[`size`](#structindex__s2_1a240d921e8475796c81141e21d76755b1) 

# struct `key` 

Struct that represents a B-Tree key. Every key has a id, file type and a rrn or byteoffset. A union "value" is used to store different type values on the key.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int `[`id`](#structkey_1a91aaee28ce9fa803f5d42fd76b189f8a) | 
`public char `[`fileType`](#structkey_1a0779ed737900b74aa1c7aa948a72d0fa) | 
`public long int `[`byteOffset`](#structkey_1a4e10ece69b96a4a8546b8069531cbf6a) | 
`public int `[`rrn`](#structkey_1a852b91ba3d7927f67f4187b9b0efbb1c) | 
`public union key::@0 `[`value`](#structkey_1af2c522dc275e560b9e2bc7cb5a613382) | 

## Members

#### `public int `[`id`](#structkey_1a91aaee28ce9fa803f5d42fd76b189f8a) 

#### `public char `[`fileType`](#structkey_1a0779ed737900b74aa1c7aa948a72d0fa) 

#### `public long int `[`byteOffset`](#structkey_1a4e10ece69b96a4a8546b8069531cbf6a) 

#### `public int `[`rrn`](#structkey_1a852b91ba3d7927f67f4187b9b0efbb1c) 

#### `public union key::@0 `[`value`](#structkey_1af2c522dc275e560b9e2bc7cb5a613382) 

# struct `registry_data_s` 

Struct that stores registry data.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int `[`id`](#structregistry__data__s_1addf22a9186e4e8e3fd1871b8dfc4b678) | 
`public int `[`year`](#structregistry__data__s_1a2adc425b2bc8ef523c6e5e91b00f4f85) | 
`public char `[`city`](#structregistry__data__s_1ab9310f74355797135b58d69e01f16c9a) | 
`public int `[`amount`](#structregistry__data__s_1a33dcabcaa533414579666d16ecdd8256) | 
`public char `[`initials`](#structregistry__data__s_1ab6b14c5d781ead0ace9f63a00d823b59) | 
`public char `[`brand`](#structregistry__data__s_1a326c4772e820877ae96ccc45a6dee5b0) | 
`public char `[`model`](#structregistry__data__s_1aa24d2377c62da75ef5d4f8da2d2544da) | 

## Members

#### `public int `[`id`](#structregistry__data__s_1addf22a9186e4e8e3fd1871b8dfc4b678) 

#### `public int `[`year`](#structregistry__data__s_1a2adc425b2bc8ef523c6e5e91b00f4f85) 

#### `public char `[`city`](#structregistry__data__s_1ab9310f74355797135b58d69e01f16c9a) 

#### `public int `[`amount`](#structregistry__data__s_1a33dcabcaa533414579666d16ecdd8256) 

#### `public char `[`initials`](#structregistry__data__s_1ab6b14c5d781ead0ace9f63a00d823b59) 

#### `public char `[`brand`](#structregistry__data__s_1a326c4772e820877ae96ccc45a6dee5b0) 

#### `public char `[`model`](#structregistry__data__s_1aa24d2377c62da75ef5d4f8da2d2544da) 

# struct `WField` 

Struct that stores searching data.

The fields of this struct are strings. Name stores name of the field and Value its value.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public char * `[`name`](#structWField_1a9c4c6ae2ad60462fb28c87206e9490fc) | 
`public char * `[`value`](#structWField_1adc4834856a1c3f695de4b5b17c15feef) | 

## Members

#### `public char * `[`name`](#structWField_1a9c4c6ae2ad60462fb28c87206e9490fc) 

#### `public char * `[`value`](#structWField_1adc4834856a1c3f695de4b5b17c15feef) 

Generated by [Moxygen](https://sourcey.com/moxygen)