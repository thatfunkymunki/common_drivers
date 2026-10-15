# Comparative reconstruction changelog

This document records the retained STB 2.4 → STB 2.6 differences in the
source-buildable modules. STB versions identify Dolby libraries, independently
of Linux versions. Linux 4.9 and 5.15 do not explain changed fixed-point math,
metadata parsing, register packing or library state machines.

| Library | Original reference SHA-256 | Algorithm bodies | Selected host |
| --- | --- | --- | --- |
| STB 2.4 | `5ffe2348b2e12d682543d275f7282fb43aedfdcc7aaa8339d229ddca6abd130b` | 238 | P3i T4c 4.9.269 |
| STB 2.6 | `f6c26659a255447685ceac9441e399c999b1fae9c6435c48d70e14a14dd7f8f7` | 321 | CoreELEC 5.15.170 and 5.15.196 |

## Source convergence decisions

These changes improve the reconstruction while preserving each original
contract. They do not claim that different native algorithms are equivalent.

- Use the same bytewise YUV offset and scalar Kr/Kg/Kb matrix implementation; retain actual LMS/IPT factors (219/224 versus uniform 219), coefficient tables and narrowing policies.
- Use independent temporary assignments and matching literal spelling. Share inverse-matrix arithmetic with common raw-32 scale inputs interpreted through their low 16 bits. Retain the signed-16 versus raw-32 singular result and truncation versus saturation.
- Share named raw arithmetic and indexed numeric-access helpers. Unify proven same-bit signed/unsigned scalar declarations, preserving signed results where maintained callers perform signed arithmetic.
- Share the six-stage raw-word logarithm polynomial, power polynomial and bounded square-root approximation. Retain the different finishing/scale branches. Restore the meaningful original manager/scale results for InitMmg and GetRgb2LmsWpByDefV3Fxp; their former STB 2.6 void declarations were unresolved decompiler artifacts.
- Share DeGamma arithmetic and byte-identical tables through direct native-owner bindings; mask the common low-16 inputs explicitly and retain the 32-bit versus 64-bit result.
- Share the low-level parser prefix/layout, lifecycle, buffering, reset, decoder dispatch, validation, supplemental input and extension code. Select actual differences through named `parser_contract.h` policies.
- Share byte alignment, residual-coefficient traversal, ST2094 inclusive target windows, synthetic defaults and byte-published extension bookkeeping. Algebra and native comparisons show that the former spellings were equivalent under their recorded backing/callback contracts.
- Share composer packing and the constant-zero unsigned-short return with pointer/dimension parameters; retain composer bases/extents, native mode selection and source preparation. Share LUT packing with native source offsets.
- Replace opaque CLI offset names with role-based priority/flag accessors. Rename control temporaries from their audited uses; retain byte offsets, operand widths, pointer reloads, partial writes and overlapping publication order.
- Use positive unsigned 32-bit kernel-stream byte counts and wrappers, raw-word minimum-frame-size results, and a no-argument constant-zero destroy helper. These preserve original return/register bits and maintained callers.
- Replace equivalent metadata endian trees, identity-prefix initializers and trial-square-root unrolling with explicit shared loads and bounded loops, retaining checkpoints, store order, count traps and partial state.
- Replace the remaining decompiler local/parameter/label names in 20 C file/version instances with audited functional roles. Preserve reused bindings with explicit multi-role names; no types, casts, operators, evaluation/store order, control flow or source line counts change. Align the tone-control prototype parameter with its actual previous-mode argument. Function-scoped lexical proofs and final linked machine/data comparisons validate this source-only cleanup.
- Share comparative parser, control, runtime, numeric and source-guide documentation. Keep native owners, layout assertions, reference identities and matching-kernel build profiles explicit.

Source text uses LF line endings. The STB 2.4 already-initialized diagnostic
was removed with its unreachable branch: original initialization clears the
tested byte before the decoder setter, which cannot modify that byte or call
the logger. Repeated serialized initialization follows the same path. This
is dead-code cleanup, not a changed reachable diagnostic policy.

## Retained parser behavior

### P01: Allocated parser instances and application layout

STB 2.4 retains a static 0x780 application context, contiguous payload backing, singleton parser and persistent external counters. STB 2.6 allocates a 0x770 context, 0x10442-byte sequence and queried parser arena, stores output sizes per instance, retains the frame counter globally and frees/nulls allocations in the native failure/release order.

### P02: Host metadata processing/reset contract

STB 2.4 accepts the historical logger/init flag, ignores reset flags, leaves release empty, passes its full eos byte and resets the parser after a successful processing call. STB 2.6 accepts an explicit instance, adds flags and input_kind, uses flags bit 0 for flush, runs VUI/ST2086/T.35 ingestion for kinds 1/2, does not reset after process success, and resets the global frame counter after successful explicit reset.

### P03: Reset flags and direct-mode deep clear

STB 2.6 adds a second flags argument to both reset APIs; flags==1 clears the current header (0x136), DM payload (0x10744) and two payload records (0xe070) in direct mode. STB 2.4 resets indices/state/MEL without those payload clears. Buffered STB 2.6 reset traps after writing the first fifteen payload handles for a larger slot count; STB 2.4 retains unchecked copying.

### P04: Buffered error draining and MEL accounting

Both parser versions stop for -20/-21 queue sentinels. STB 2.4 reports the first other nonpositive output once and stops, and maps any nonzero decoder MEL byte to flag bit 0. STB 2.6 continues draining after other errors according to its native unsigned loop condition, retaining the last error result, and ORs the raw MEL byte into caller flags.

### P05: Fatal limits and native profile-16 row

STB 2.6 retains fatal checks for decoder/query level>=16; validation level 16 uses BRK #1 and larger levels BRK #0x5512, profile>16 is fatal while profile 16 uses its separate retained 38-word row, mapping pivot15 is fatal, payload IDs>=15 are fatal, spatial modes>=8 are fatal and sentinel searches retain whole-row/table bounds. Buffered byte append increments length before trapping for previous length>0xfff. STB 2.4 retains its -4/-5 validator returns and unchecked pointer/array paths, including write-before-post-check behavior in buffering.

### P06: Sparse identity fallback writes

STB 2.4 fallback initializes each complete 28-byte NLQ record. STB 2.6 preserves the native sparse/overlapping store sequence at 0x6874..0x68c7 and exposes set_default_payload as a separate native entry. Common polynomial defaults and bytewise access do not erase the differing untouched NLQ bytes.

### P07: Decoder diagnostics and API strings

Parser API text changes from 0.4.1 to 0.4.3 while algorithm text remains 1.1.2. STB 2.4 keeps algorithm text at API text+8; STB 2.6 keeps separate string objects. Diagnostic source path changes to the relative Dolby source path; header/payload/DM diagnostic lines change 0x225/0x236/0x243 to 0x22a/0x23b/0x248, and the extension diagnostic base changes 0x581 to 0x585. STB 2.4 omits the partition-count printf argument; STB 2.6 supplies it. Module diagnostics retain their native logger/import selection and existing reconstruction prefixes.

### P08: Extension callback interface and record selection

STB 2.4 extension payload reading receives explicit configuration, event-callback slot and typed reader; STB 2.6 receives the decoder and derives those dependencies from it. Both only read the byte index, which is now const in both signatures. Non-first payload diagnostics test kind==2 in STB 2.4 versus kind!=1 in STB 2.6. Unknown extension payload copy is unchecked by the 256-byte union limit in STB 2.4 and stops after byte 255 in STB 2.6. Equivalent local index-width and kind type spelling differences have been removed.

### P09: DM extension bit-count and sequence-offset accounting

STB 2.4 adds initial extension read results and second incremental-update read results to initial_bits before computing its returned bit count; STB 2.6 omits those adjustments. For unknown packed DM extension payloads, STB 2.4 addresses output through signed 32(position+5+offset), retaining wrapping32-bit arithmetic; STB 2.6 addresses block[5+offset] after selecting the block pointer. This is serialization addressing, not sequence-header parsing.

### P10: NLQ sign-mask truncation

For the retained NLQ coefficient path, STB 2.4 uses1u<<((width+1)&31); STB 2.6 uses2u<<(width&31). When width&31==31, STB 2.4 selects bit 0 while STB 2.6 drops the sign mask to 0. PARSER_NLQ_TRUNCATING_SIGN_SHIFT names this actual arithmetic difference; equivalent one-bit-reader helper selection has been removed.

### P13: Native entry visibility and historical VUI argument

STB 2.4 keeps the payload-level MEL predicate private and accepts an ignored VUI pointer in decoder/parser VUI calls. STB 2.6 exposes the payload-level MEL entry and removes that unused VUI argument. The shared implementation preserves these native API contracts through explicit declarations.

### P14: Metadata callback copy-length widening

After the same signed capacity comparison, STB 2.4 widens metadata copy length through unsigned 32 bits; STB 2.6 sign-extends it to 64 bits. Normal serializer lengths are positive, but the native distinction remains explicit for nonpositive abnormal lengths and partial-output behavior.

## Retained control behavior

### C-control-entry

STB 2.4 has one source, separate graphics/target descriptors and an explicit runtime storage binding. STB 2.6 accepts three 0x68-byte private inputs in a 0x4f58-byte settings record, selects a primary, stages multiple signal/kernel contexts and publishes two Core 1 result slots. This changes source selection and callback shape independently of the kernel.

### C-control-lifecycle

STB 2.4 resets on input format -1 and calls its no-op destroy helper on ordinary completion without clearing initialized. STB 2.6 resets on zero input count, clears native staging and initialization/content bytes, and retains shared control state across ordinary calls. Init_cp takes instance/configuration inputs, rejects counts >=5 and retains native zero/negative-count behavior. The destroy helper now has the same no-argument declaration and zero-result body; its old 2.4 arena argument was ignored by the native instructions.

### C-manager

The 2.4 manager has 32 request slots and signed unchecked indexing after its diagnostic callback and sizes a singleton arena. The 2.6 manager has 24 slots, reloads its count between selected writes, returns -1 at the exact threshold and traps for further/native unsigned bounds, retains partial writes, and sizes per-instance kernel/extensions. InitMmg returns the manager pointer in both native instruction streams and now has one pointer-return declaration; the prior 2.6 void spelling was a decompiler choice, not a native ABI difference. RegMemReq uses a common unsigned in-device word: each native implementation merely stores those 32 bits and maintained callers pass boolean values.

### C-getters

The 2.4 configuration getter selects pending or active validity and its stored pointer; GetHDmKs reads the singleton pointer. The 2.6 configuration getter returns context+8 when nonnull, kernel access uses the native signed index-minus-one slot, and run/signal/TMM getters select native instance records with their original failure/trap behavior. Get_dm_ctx_buf is also a native distinct pointer calculation/load. Kernel-buffer publication forwards the common unsigned byte count; only 2.6 accepts an instance index and traps at index >=4.

### C-frame-size

Native gray color is 4 in 2.4 versus 5 in 2.6; planar 2.4 sizing includes chroma while 2.6 gray omits it. Minimum sizing now uses one unsigned-word argument/result declaration and a common wrapped-product/signed-quarter-division expression. The native difference is only the gray omission condition, not a decompiler signedness artifact. Compact row-pitch sizing still halves each signed dimension before its products, unlike minimum sizing.

### C-vsvdb

The 2.4 entry writes an 88-byte target descriptor and returns -1 for null/invalid identification. The 2.6 helper writes its control-state capability fields, treats null as retained state/success and returns -8 for invalid identification/version/length. Each keeps its original capability/luminance partial writes. Luminance units and rounding differ; in particular 2.6 v1 squares/left-shifts a u32, reinterprets signed and divides toward zero. Original instructions at 0015c4b4..0015c4c4 confirm signed lowering. Proven equivalent packed-primary expressions now use the clearer common spelling.

### C-composer

Shared composer packing keeps a constant-zero unsigned-short result and pointer/dimension arguments. The native composer base/buffer extent are 176/900 bytes in 2.4 versus 200/968 in 2.6. Mode is residual_flag!=0 versus constant zero. Width/height originate from 2.4 source+12/+16 versus 2.6 source+0/+4. STB 2.6 can change selected 8-bit BL/EL configuration and coefficients to 10 bits in place before packing. Interleaved loads/stores remain valid for its overlapping views; 2.4 requires disjoint objects.

### C-lut

The four 15-bit paired LUT streams and final 32-bit input-map copy now have one implementation and pointer interface. Callers select tone/input bases kernel+1328/+200 in 2.4 versus kernel+0x540/+0xd8 in 2.6. Output order and 2.6 overlap behavior are preserved.

### C-hdr

The 2.4 infoframe builder uses its input-format/HDR10 arguments and fixed primaries. The 2.6 builder derives primaries from fixed-point target words and its publisher applies native primary-input HDR10 passthrough/debug selection. For level-6 extensions, 2.4 processes matching records in order whereas 2.6 selects the last record first. Optional output-format getters expand from required 16-byte records to native PQ/luminance/primaries records in 2.6.

### C-metadata-storage

The 2.4 control representation has a 71-byte prefix and fixed 261-byte extension slots in current/previous 66626-byte backing, optional temporal level-1 filtering, logger failure for length>256 and native HDMI trim/removal/compaction. The 2.6 outer control path consumes contiguous variable-length records in 0x1000-byte slots, retaining native signed offsets, incomplete-header reuse, per-source progress and fatal capacity checks after partial writes. It has two metadata/composer staging slots despite three private host input records.

### C-publication

The 2.4 raw HDMI publisher calculates size before payload writes, caps >512 words, logs and preserves the previous payload after availability is cleared. The 2.6 publisher retains native raw-word and extension writes interleaved with fatal capacity checks, publishes transport control data and content information, and preserves register/LUT slot selection. Core register/host field offsets and buffer extents differ. HDR publication now uses four explicit LE64 copies at offsets 0,8,16,21, retaining the final overlapping store.

### C-transport-content

STB 2.6 adds VSEM/VSIF packet decoding, packet header/OUI validation and -5 failure paths, transport-derived signal configuration, optional Dolby control packets, and content/white-point/level-11 callback notification. Packet/type/status writes and callback sequences remain ordered. The 2.4 callback path instead uses the native packed metadata and separate HDR10/source arguments.

### C-graphics

The 2.4 graphics path derives a separate 64-byte graphics descriptor from CLI fields and applies stored luminance offsets. The 2.6 path uses native input_mode 2, format defaults/primaries, graphic luminance words and a separate Core 2 slot. Native gamma, matrix, source/default field writes and partial-state order remain version-specific.

### C-initializers

STB 2.4 initializes pending/active DM pointers, its 168-byte tone-control fields and trim arrays with retained self-pointers/defaults. STB 2.6 initializes native multi-signal/active kernel state, a larger tone-control record and extension descriptors. InitMdsExt now has the same pointer declaration; 2.6 intentionally ignores configuration while 2.4 reads it. Counts, optional writes, identity LUT initialization and fatal paths are retained.

### C-data-ownership

STB 2.6 owns canonical contiguous 210856-byte control storage, signature storage and constant/table members with explicit reserved spans and offset/size assertions. STB 2.4 keeps its separately aligned runtime owners and table objects. Storage owners and native relative aliases remain separate; identical table includes retain shared values without merging instances. Version/build/diagnostic strings remain the pinned native 2.4/2.6 identities.

### C-cli

Both modules use bytewise CLI priority accessors. Priority is byte 52 in the 112-byte 2.4 record versus byte 0x1d14 in the 7468-byte 2.6 record. The 2.6 tail additionally contains selected input count/primary, native reserved init/input flags and level-11 controls. Unknown priority-override/init/input fields remain explicitly reserved; no speculative whole-struct overlay was introduced.

## Complete conversion-path ledger

Each F entry describes one path that still differs in the conversion.
The review artifact maps every exact Git hunk to these entries. Native
layout/API and added/removed algorithm differences are retained explicitly;
source organization and integration metadata are classified separately.
Shared files are absent from this path list. Some shared implementations
still select the actual parser, composer, numeric or CLI policy described above.

### F001: `README.md`

Classification: documentation/provenance.

Identify the selected STB library, source hash, algorithm coverage and native build/host contract; link the exhaustive comparative changelog.

- STB 2.4 reference5ffe2348...130b implements238 algorithm entries; STB 2.6 referencef6c26659...8f7 implements321. These library identifiers are separate from kernel versions.
- Selected build and host boundary differs: T4c4.9.269 singleton versus CoreELEC5.15.170/.196 multi-instance, with their matching generated kernel inputs.

### F002: `build_profile.mk`

Classification: kernel-build/compiler integration.

Select each matching compiler and host-header build interface while emitting the same dovi.ko target.

- 4.9 uses AArch64 GCC, GNU11 and -fno-builtin with in-tree host declarations. 5.15 uses Clang/LLVM and integrated assembler, common-drivers includes, -fwrapv and -fno-strict-aliasing.
- 5.15 direct callbacks require CONFIG_CFI_CLANG=n; generated CRCs, instrumentation and stack protection come from the selected kernel.

### F003: `callback_registration.c`

Classification: host ABI/lifetime contract.

Publish persistent callback storage in the host's actual named slots and preserve unsupported null entries.

- 4.9 publishes a const48-byte table containing version/init/reset/process/release/control. 5.15 publishes a mutable112-byte host object with 96-byte STB prefix, multi-control and multi-parser callbacks, and null TV/HW5 entries.
- 5.15 receives the version pointer at registration; the complete object remains resident even after some host registration failures.

### F004: `core/byte_utils.c`

Classification: native edge behavior.

Preserve different negative-count semantics in otherwise identical forward byte-copy/equality loops.

- 4.9 uses signed count>0: negative counts copy nothing and compare equal. 5.15 sign-extends the 32-bit count to 64 bits and loops until zero: negative counts become huge lengths and can fault or encounter a mismatch.
- Forward-copy overlap propagation remains shared; neither lane is hardened or replaced with memmove.

### F005: `core/byte_utils.h`

Classification: native edge contract declaration.

Declare the same signed-count API and document its version-specific negative-count behavior.

- Negative count skips/equal in 2.4; huge sign-extended count in 2.6. Buffer extent and forward overlap requirements remain explicit.

### F006: `core/color_basis.c`

Classification: native-vendor-algorithm-and-contract.

Four element-copy/row-sum operations share color_basis_impl.h with identical bodies and alias/reload rules; fixed LMS/IPT getters now also share their unsigned 32 result declarations because the results are exactly12/15.

- Four element-copy/row-sum operations share color_basis_impl.h with identical bodies and alias/reload rules; fixed LMS/IPT getters now also share their unsigned 32 result declarations because the results are exactly12/15.
- STB 2.6 adds cross-talk and inverse-cross-talk matrix constructors, XYZ-to-LMS constants, ICtCp IO/DM forward/inverse bases and their scale results. The arbitrary-scale inverse retains zero-divisor AArch64 behavior.

### F007: `core/color_basis.h`

Classification: native-declarations-data-or-private-helper-organization.

Four element-copy/row-sum operations share color_basis_impl.h with identical bodies and alias/reload rules; fixed LMS/IPT getters now also share their unsigned 32 result declarations because the results are exactly12/15.

Details shared with: F006.

### F008: `core/color_narrow.c`

Classification: native-vendor-algorithm-and-contract.

YUV offset and scalar Kr/Kg/Kb-to-YUV/RGB matrix bodies and declarations are identical across versions, with explicit bytewise stores, raw signed divide and kg=0 preservation of coefficients four/five.

- YUV offset and scalar Kr/Kg/Kb-to-YUV/RGB matrix bodies and declarations are identical across versions, with explicit bytewise stores, raw signed divide and kg=0 preservation of coefficients four/five. Narrow matrix result widths were normalized after finite shift-class bounds proved all scale results positive.
- STB 2.4 LMS/IPT narrowing applies 219 luma and 224 chroma factors; STB 2.6 applies 219 uniformly. Bits=10 matrix outputs prove this is independent of saturating narrowing.
- STB 2.4 supplies general full/narrow RGB/YUV helpers with explicit signed 16 scale/depth views; STB 2.6 adds ICtCp narrow IO matrices and retains its different unusual-depth paths. Coefficient packing remains truncate versus saturate; that is independent of the now-common bounded scale result types.

### F009: `core/color_narrow.h`

Classification: native-declarations-data-or-private-helper-organization.

YUV offset and scalar Kr/Kg/Kb-to-YUV/RGB matrix bodies and declarations are identical across versions, with explicit bytewise stores, raw signed divide and kg=0 preservation of coefficients four/five.

Details shared with: F008.

### F010: `core/color_primaries.c`

Classification: native-vendor-algorithm-and-contract.

The independent temporary[5]/temporary[6] stores and scale 26 literal now use matching presentation.

- The independent temporary[5]/temporary[6] stores and scale 26 literal now use matching presentation. Other RGB/YUV full matrices retain their original signed/unsigned division paths and coefficient sources.
- STB 2.4 reads a seven-definition RGB-to-XYZ table; STB 2.6 has separate Kr/Kg/Kb constants whose low-order coefficients can differ, plus19 primary records. Raw initializer values were preserved.
- STB 2.6 adds descending sort, xy/CCT and xyY/XYZ conversions, primary lookup and custom-primary matrix builders with native determinant masks, bounds traps and output widths.

### F011: `core/color_primaries.h`

Classification: native-declarations-data-or-private-helper-organization.

The independent temporary[5]/temporary[6] stores and scale 26 literal now use matching presentation.

Details shared with: F010.

### F012: `core/color_primaries_tables.h`

Classification: native-declarations-data-or-private-helper-organization.

The independent temporary[5]/temporary[6] stores and scale 26 literal now use matching presentation.

Details shared with: F010.

### F013: `core/composer_contract.h`

Classification: Dolby version contract, native layout or binding.

Select only the native composer base, buffer extent and mode rule.

Details shared with: C-composer.

### F014: `core/control_abi_recovered.h`

Classification: Dolby version contract, native layout or binding.

Named STB 2.4 control/source/graphics/target, configuration, tone, extension, manager and kernel offsets; the distinct 2.6 views live in its layout/workspace headers.

Details shared with: C-control-entry, C-manager, C-initializers, C-metadata-storage, C-getters, C-graphics.

### F015: `core/control_cli_contract.h`

Classification: Dolby version contract, native layout or binding.

Explicit native CLI offsets and honest reserved names.

Details shared with: C-cli.

### F016: `core/control_graphics.c`

Classification: Dolby version contract, native layout or binding.

STB 2.4 separate graphics conversion; uses common LE word helpers instead of duplicate byte loops.

Details shared with: C-graphics.

### F017: `core/control_memory.c`

Classification: Dolby version contract, native layout or binding.

Native manager capacities, arena sizing and indexed accessors; common unsigned request words and wrapped-size algorithm retain only the native gray omission difference.

Details shared with: C-manager, C-getters, C-frame-size, C-data-ownership.

### F018: `core/control_memory.h`

Classification: Dolby version contract, native layout or binding.

Common manager-pointer return and unsigned frame-size/request words remove decompiler type churn; native manager count and indexed getter contracts remain.

Details shared with: C-manager, C-getters, C-frame-size.

### F019: `core/control_metadata.c`

Classification: Dolby version contract, native layout or binding.

STB 2.4 fixed-stride packed reader, temporal filter and HDMI metadata preparation; 2.6 uses native contiguous ingress and its DM output coordinator.

Details shared with: C-metadata-storage, C-publication.

### F020: `core/control_metadata.h`

Classification: Dolby version contract, native layout or binding.

Keep the audited 2.4 packed-reader/filter/HDMI entry contracts and native logger boundary.

Details shared with: C-metadata-storage.

### F021: `core/control_orchestration_bindings.h`

Classification: Dolby version contract, native layout or binding.

STB 2.6 canonical native storage/field bindings and narrow adapters.

Details shared with: C-control-entry, C-data-ownership, C-cli, C-transport-content.

### F022: `core/control_packets.c`

Classification: Dolby version contract, native layout or binding.

Native infoframe, output-format/kernel getters, defaults and 2.6 transport/content helpers; common fixed-load64 pointer reads, unsigned dump byte counts and LE32 infoframe header writes remove equivalent spelling debt.

Details shared with: C-hdr, C-transport-content, C-getters, C-graphics, C-vsvdb.

### F023: `core/control_packets.h`

Classification: Dolby version contract, native layout or binding.

Native packet/getter declaration differences follow actual source inputs, optional outputs and indexed streams; kernel dump results share the unsigned byte-count type.

Details shared with: C-hdr, C-transport-content, C-getters.

### F024: `core/control_path.c`

Classification: Dolby version contract, native layout or binding.

Retain singleton versus multi-input orchestration, reset, source selection and partial metadata writes.

Details shared with: C-control-entry, C-control-lifecycle, C-metadata-storage, C-cli, C-graphics, C-publication, C-transport-content.

### F025: `core/control_path.h`

Classification: Dolby version contract, native layout or binding.

Distinct native control entry contracts: explicit 2.4 storage/17 arguments versus a 2.6 settings record.

Details shared with: C-control-entry, C-control-lifecycle.

### F026: `core/control_publish.c`

Classification: Dolby version contract, native layout or binding.

Retain host-native register/LUT placement, raw metadata capacities, extension availability and HDR/content publication; name cursors and remove packed HDR concatenation noise.

Details shared with: C-publication, C-hdr, C-transport-content.

### F027: `core/control_publish.h`

Classification: Dolby version contract, native layout or binding.

Native output publication functions; extracted helpers use pointer interfaces, including content-info storage slots.

Details shared with: C-publication, C-hdr, C-transport-content.

### F028: `core/control_runtime.c`

Classification: Dolby version contract, native layout or binding.

STB 2.4 aligned singleton storage owner and explicit service binding, superseded by the native 2.6 canonical storage owner/glue.

Details shared with: C-control-lifecycle, C-data-ownership.

### F029: `core/control_runtime.h`

Classification: Dolby version contract, native layout or binding.

STB 2.4 explicit service/storage API; native 2.6 service entry types remain in its memory/state bindings.

Details shared with: C-control-lifecycle, C-data-ownership.

### F030: `core/control_setting_layout.h`

Classification: Dolby version contract, native layout or binding.

Native host settings/output offsets, private input strides and packed HDR scratch union/assertions.

Details shared with: C-control-entry, C-publication.

### F031: `core/control_state.c`

Classification: Dolby version contract, native layout or binding.

Preserve each native initializer, tone/extension defaults, indexed state getter and no-op destruction contract.

Details shared with: C-control-lifecycle, C-initializers, C-getters, C-data-ownership.

### F032: `core/control_state.h`

Classification: Dolby version contract, native layout or binding.

Retain actual native initialization/count/index/callback contracts and buffer extents; no-op destruction has one no-argument declaration and zero result.

Details shared with: C-control-lifecycle, C-initializers, C-getters, C-transport-content.

### F033: `core/control_target.c`

Classification: Dolby version contract, native layout or binding.

Decode native VSVDB capabilities and configure output mode; use proved common bit-field arithmetic without altering native unit/rounding or partial-state order.

Details shared with: C-vsvdb, C-graphics, C-hdr.

### F034: `core/control_target.h`

Classification: Dolby version contract, native layout or binding.

2.4 descriptor conversion and 2.6 context decoding/configuration are distinct native interfaces; buffers use pointer types.

Details shared with: C-vsvdb.

### F035: `core/control_workspace.h`

Classification: Dolby version contract, native layout or binding.

STB 2.6 local workspace aliases have asserted native offsets/sizes; preserve overlapping metadata/primaries and scratch-word views.

Details shared with: C-control-entry, C-metadata-storage, C-transport-content.

### F036: `core/data_layout.h`

Classification: Dolby version contract, native layout or binding.

Native 2.6 storage and table ownership maps; explicit reserved spans maintain aliases and complete compile-time layout assertions.

Details shared with: C-data-ownership, C-control-entry, C-cli.

### F037: `core/detect_mel.c`

Classification: retained vendor API/layout/algorithm delta.

MEL predicate is identical; native payload-entry visibility remains selected by the library interface.

- P13

### F038: `core/dm_backlight.c`

Classification: added vendor algorithm/state.

Add adaptive backlight filtering and global-dimming result getters to STB 2.6.

- ApplyAblu selects indexed or primary state, restores target windows when inactive, derives filtered PQ/linear anchors, clamps model inputs, applies asymmetric temporal weights and frame thresholds, and updates target/output ranges.
- Result getters require an active state, permit null output pointers, return zero or zero-extended 0xffffffff, and expose filtered anchors/means, target window and backlight model input/output. No STB 2.4 standalone counterparts exist.

### F039: `core/dm_bindings.h`

Classification: native API/data binding organization.

Bind version-specific DM stages to concrete numerical, control, metadata and table owners.

- 2.4 aliases single-input commits and typed kernel/configuration storage. 2.6 aliases indexed consolidation, metadata output, local trim/global-dimming stages and canonical retained constant members.
- OS memory and diagnostics enter through the corresponding runtime service table; aliases do not merge mutable state or change its address/lifetime.

### F040: `core/dm_commit_pipeline.c`

Classification: vendor control/state-machine behavior.

Replace single pending/active configuration commit orchestration with indexed multi-input configuration/metadata/tone/map/global-dimming sequencing.

- 2.4 CommitMds selects forced/current metadata, commits pending color/graphics/video/tone/chroma state, swaps active/pending configuration pointers, and dispatches dynamic metadata.
- 2.6 Commit2DmKs treats null metadata and the all-ones metadata sentinel differently, classifies700-byte records and 120-byte prefixes, maintains per-input flags and primary-mode timing, consolidates input, chooses bypass or concrete maps/tone curves, and finalizes stored metadata.
- 2.6 preserves diagnostics, temporal transitions, inverse-map de-duplication, backlight dispatch, native return bits and partial state updates; a whole-body unification would change sequencing.

### F041: `core/dm_commit_tables.inc`

Classification: raw data ownership/provenance.

Keep STB 2.4 private chroma/filter/coefficient table definitions; STB 2.6 binds its independently recovered constants through the canonical aggregate.

- The conversion removes the 2.4 table-owner include; values and reachable table bindings remain version-specific source data, not precompiled processing objects. Raw original initializer values are preserved.

### F042: `core/dm_configuration.c`

Classification: native configuration/layout/defaults.

Preserve each native signal/control/configuration initializer and getter while using named views where verified.

- 2.4 initializes separate source/graphics/target layouts with sparse writes, retained Chrm/pitches and active-or-pending configuration getters. 2.6 initializes indexed 0x120-byte signals,0x34-byte alpha records,0x64-byte control and 0x1b0-byte tone control in the 0x8e4 configuration prefix.
- 2.6 adds global-dimming defaults and direct context getters: six-stride legacy signal views, null-context failure, fatal nonnull out-of-range signal access, and zero-extended 0xffffffff status. Its five full signals plus alpha region remain distinct from the legacy index5 byte view.
- Initializer clearing, primary index, transfer defaults, primaries callbacks, manager pointer storage and repeated writes retain their native order and untouched fields.

### F043: `core/dm_configuration.h`

Classification: native scalar/buffer API contract.

Declare the distinct configuration and signal APIs rather than erase argument counts, result widths or record types.

- 2.6 source initializers add a signal index; configuration initialization ignores its firstLP64 argument, accepts word-backed storage and returns its pointer. Getters use indexed or direct native context fields and LP64 zero-extended status values.
- 2.4 retains its typed DWARF-derived records and active/pending selection; these are different object layouts and operations, not merely alternate names for an identical structure.
- GetTmCtrl/GetDmCtrl now share signed 32 status-result spelling: both native entries return0 or zero-extended 0xffffffff. Only object/context parameter types and the different selection/copy contracts remain; their former64-bit result spelling was recovery churn.

### F044: `core/dm_extensions.c`

Classification: added vendor helper entry.

Add the 2.6 append entry around the already shared extension-record copy primitive.

- The additional function copies count fixed0x108-byte records after the current byte count and then updates that byte count with native wrapping. No new allocation, capacity hardening or record conversion is introduced.

### F045: `core/dm_frame_format.c`

Classification: native layout/partial-write behavior with representation cleanup.

Use clear common field encodings and sample/component arithmetic while preserving version-specific geometry and partial updates.

- 2.4 includes compact frame sizing and singleton frame-state updates; native gray value is 4 and planar sizing retains chroma. 2.6 uses gray5 and its compact sizing omits planar-gray chroma.
- CommitCdm2KsFrmFmt source field offsets differ;2.6 writes geometry/location/pitches before testing flags bit 1, then updates type/weave/bit-depth/column pitch.2.4 has no partial-update flags argument and retains its own write order.
- 2.6 metadata-to-frame conversion uses the metadata-valid bit,8-bit-versus-wider sample sizing and native weave mapping. Reloads after writes preserve overlapping views.

### F046: `core/dm_frame_format.h`

Classification: native enum/API contract.

Share sample-byte and interleaved-component helpers; retain gray4/5 and the different version-specific frame entry points.

- RGBA2,422chroma1, U8sample1 andF32sample2 agree; gray is 4 in 2.4 and 5 in 2.6.2.6's configuration frame commit adds flags and uses indexed signal layouts.

### F047: `core/dm_input_consolidation.c`

Classification: vendor numerical/control algorithm.

Retain the substantially different color, crop, user-adjustment and trim consolidation pipelines.

- 2.4 commits singleton external/default LMS/IPT matrices and configuration-derived video/graphics tone targets, including native external inverse-scale behavior and HLG endpoints.
- 2.6 selects indexed versus primary state; crops exclusive active-area endpoints with required dimension reloads; clamps contrast/chroma/saturation/luminance adjustments and signed white-point offsets; selects or constructs primary matrices and color-temperature transforms.
- 2.6 coordinates level 2/8 trim interpolation, local-ms adjustments and metadata/configuration fallbacks through overlapping typed scratch views, retaining input flags, native pointer selections, table bounds, rounded/scaled arithmetic and intermediate stores.
- The top-level tail selects level-254 mode/version from metadata+0x6f/+0x70 (version clamps value+2 to 4), and the +0xd8 selector from metadata+0x71, configuration or primary state. Selector2 returns after active-area consolidation; signed control word+0xdc>127 clears flags0x210. Tone-control preparation precedes trim consolidation.

### F048: `core/dm_input_map.c`

Classification: native-vendor-algorithm-and-contract.

STB 2.4 commits singleton graphics/video metadata, their color matrices and offsets through native configuration/context fields.

- STB 2.4 commits singleton graphics/video metadata, their color matrices and offsets through native configuration/context fields. STB 2.6 splits metadata-map, signal-map and indexed IPT/ICtCp matrix preparation, with independent color/range bypass decisions and DM3 bypass builders.
- Map workspace sizes, matrix destinations, cached versus reloaded pointer paths, packed 16/32/64 accesses and partial writes are native layout/sequencing contracts; copying either whole body to the other would change the interface and output records.

### F049: `core/dm_input_map.h`

Classification: native-declarations-data-or-private-helper-organization.

STB 2.4 commits singleton graphics/video metadata, their color matrices and offsets through native configuration/context fields.

Details shared with: F048.

### F050: `core/dm_kernel_maps.c`

Classification: native kernel-state/configuration behavior with representation cleanup.

Share the metadata classifier shape and replace decompiler-only square-root unrolling while preserving all version constants and commit semantics.

- InitDmKs changes row-pitch 0x1000->0x2000, allocation 0x32a0000->0xca80000 and manager/config offsets, stores the 2.6 context handle, and initializes different native kernel maxima/storage.
- CheckMds compares 520/514 bytes and returns 0/1/2 in 2.4;2.6 compares 700/120 and returns 0/4/1. These changes are confirmed against both original binaries.
- 2.4 CommitCfg reads its pending pointer before the nominal null check, copies 984-byte pending configuration, and updates singleton frame addresses.2.6 indexes kernel/metadata state, initializes global dimming/filter timing and integer square root, writes frame-buffer planes at+0x4380000/+0x8700000, commits indexed/alpha/target frames and linked output-map pointers.
- 2.6 CommitFinalized emits first/primary/last input bits, marks initialized, copies 0x8f8 bytes, stores the committed mode and conditionally disables dimming.

### F051: `core/dm_layout.h`

Classification: native byte layout/enums.

Describe the independently recovered2.4 full typed layouts and 2.6 verified signal, metadata, control, frame, active-area and dimming views.

- Configuration, context, kernel and metadata field offsets/sizes differ.2.6 adds indexed signal/context strides, primary-mode fields, five full signals plus alpha storage, level 8/9/10/11/254 fields and global-dimming results.
- Native MDS classifier extents/results are explicitly named in both versions; asserted sizes/offsets are retained. Unconfirmed regions keep reserved/offset names rather than borrowed 2.4 semantics.

### F052: `core/dm_map_workspace.h`

Classification: added native workspace representation.

Name2.6 map/commit scratch storage and constrain overlapping16/32/64-bit views with layout assertions.

- These locals belong to the indexed 2.6 map/commit algorithms; typed unions retain aliasing and native extents rather than represent newly added persistent state or a kernel ABI.

### F053: `core/dm_metadata.c`

Classification: native metadata decoding with representation cleanup.

Use shared big-endian readers and sequential header checkpoints; retain distinct legacy/indexed extension decoding and partial writes.

- 2.4 decodes the 71-byte base into 520-byte typed metadata with caller-owned trim arrays, signed remaining lengths, level 1/2/3/5/255 handling and count/pointer reloads that can alias.
- 2.6 resets decoded flags/scales and primary-input unknown-record cursors before parsing, fills its700-byte indexed format, and handles level 1/2/3/4/5/6/8/9/10/11/254/255 with per-level capacity checks and fatal trim-array bounds.
- Level8 optional fields depend on declared lengths and local-ms defaults; level 9/10 custom primaries and unknown tails retain their separate pointer/length arrays. Counts, duplicate/order handling, skipped bytes, zero/negative/wrapped size predicates, partial updates and diagnostics remain native.

### F054: `core/dm_metadata_output.c`

Classification: added vendor output-metadata algorithm with representation cleanup.

Add 2.6 output selection/measurement, fixed-header generation and staged extension serialization with native capacity/partial-state semantics.

- GetMdsOutput requires initialized context, applies configured mode/capacity fallback and optional measure-before-write; GenerateOutputMd selects indexed/primary signal and source state, exclusions and target/signal versions.
- Fixed-header generation retains matrix/EOTF/primaries/native minimum-maximum derivation and state updates before capacity failure. Legacy level 1/2/3/4 emission includes target-version constraints and trim selection; identity-prefix initialization retains signed count and a fatal16-entry bound after the first 16 writes.
- Level8 uses indexed 13-byte core plus optional16-bit/byte fields and unknown tails; level 9 chooses indexed/custom primaries; level 10 emits indexed 6/22-byte records with unknown fields; L11 supports source/override fields; L254 and aggregate unknown records keep separate gates, payload lengths and count timing.
- Null-output measurement, staged endian writes, duplicate record selection, signed capacity behavior, extension-count increments, diagnostics and trap/partial-output boundaries are preserved.
- This output generator has dedicated emitters for levels1/2/3/4/8/9/10/11/254 and aggregate unknown records. Level255 decoding belongs to dm_metadata.c; it is not a dedicated output emitter here.

### F055: `core/dm_output_map.c`

Classification: native-vendor-algorithm-and-contract.

STB 2.4 takes config/update-minmax/context for the singleton map; STB 2.6 selects a signal-indexed output owner and updates mode-specific target min/max, ICtCp IO matrices and cross-talk state.

- STB 2.4 takes config/update-minmax/context for the singleton map; STB 2.6 selects a signal-indexed output owner and updates mode-specific target min/max, ICtCp IO matrices and cross-talk state.
- STB 2.6 adds UpdateKsOMapTmm and DM3 output bypass; native output formats, packed store widths and owner-pointer reloads remain distinct. Shared YUV range/offset and matrix primitives now carry equivalent suboperations.

### F056: `core/dm_output_map.h`

Classification: native-declarations-data-or-private-helper-organization.

STB 2.4 takes config/update-minmax/context for the singleton map; STB 2.6 selects a signal-indexed output owner and updates mode-specific target min/max, ICtCp IO matrices and cross-talk state.

Details shared with: F055.

### F057: `core/dm_pipeline.h`

Classification: native orchestration/API contract.

Declare singleton 2.4 orchestration versus 2.6 indexed metadata, output, trim, consolidation, dimming and finalization APIs.

- Different argument counts, signal indices, sentinel/null handling, object extents and native scalar result widths remain explicit.2.6 adds output generators and global-dimming getters;2.4 retains its pending-pointer-before-null-check contract.

### F058: `core/dm_transfer.c`

Classification: native transfer behavior with representation cleanup.

Share the DeGamma calculation and scalar input forms, common context pointer and primary-direction logic; retain indexed transfer selection and native 32/64-bit results.

- 2.4 reads singleton tone-direction state and separately selects graphics configuration.2.6 selects primary/indexed context by committed/current mode and signal index, preserves zero-index normalization and conditional fatal bounds, and adds indexed/primary getters plus EOTF propagation.
- DeGamma uses one common calculation with five unsigned 32 inputs, four explicit low16 reductions, unsigned 32 log interpolation/wrap, identical18442table elements, conditional64-bit overflow scaling with 32-bit truncation, and a 64-bit final product. Wrappers bind the existing2.4private and 2.6 aggregate table owners and return low32 versus full64 bits. Wider input declarations and differing arithmetic/control shapes were recovery churn; the 2.6 fatal input-index guard was removed after proving it unreachable for every scalar input.

### F059: `core/dm_transfer.h`

Classification: native scalar/API contract.

Use common const opaque context pointers and unsigned 32 DeGamma inputs, retaining the 32/64-bit result difference and 2.6-only indexed transfer APIs.

- DeGamma has identical five input declarations: gain/offset/exponent/sample use low16 bits and overflow_scale uses full32. This removes a harmless recovered input-width discrepancy while preserving the 2.4 unsigned 32 versus 2.6 unsigned 64 result contract; indexed/EOTF APIs remain2.6-only.

### F060: `core/dm_trim.c`

Classification: native-vendor-algorithm-and-contract.

STB 2.4 clamps level 2 controls and commits bracket-interpolated trim values plus default/config adjustments.

- STB 2.4 clamps level 2 controls and commits bracket-interpolated trim values plus default/config adjustments. STB 2.6 instead selects up to four kept indices from luma/chroma brackets, constructs L2/L8 default trims and resolves target maximum through display/L10 lookup.
- Equal-key sort ordering, level 2/8 record sizes14/42, default injection, native array limits and diagnostics/partial writes remain part of their respective vendor APIs.

### F061: `core/dm_trim.h`

Classification: native-declarations-data-or-private-helper-organization.

STB 2.4 clamps level 2 controls and commits bracket-interpolated trim values plus default/config adjustments.

Details shared with: F060.

### F062: `core/dm_trim_chroma.c`

Classification: native-vendor-algorithm-and-contract.

Both distance pipelines now finish with the identical shared fixed_abs32 sum, preserving INT32_MIN saturation and unsigned 32 addition.

- Both distance pipelines now finish with the identical shared fixed_abs32 sum, preserving INT32_MIN saturation and unsigned 32 addition. Their preceding primary/XYZ/ICtCp computation and persistent writes remain native.
- STB 2.4 writes its116-byte chroma scratch/state and computes its own primary coefficients; STB 2.6 calls native XYZ/ICtCp helpers and returns 0 for null primaries. The former64-bit luminance spelling is normalized to unsigned 32 because every path discards its high32 bits before arithmetic.
- STB 2.6 adds chroma-coordinate construction from display/L10/metadata primaries, duplicate-coordinate suppression, default exclusion and bounded17-entry scratch workspaces.

### F063: `core/dm_trim_interpolation.c`

Classification: native-api-or-table-family-added-removed.

STB 2.6-only APIs select lower/upper interpolation indices, sort coordinates with associated indices, construct default L2/L8 records, choose native metadata-record pointers and return Q12 weights/default-use flags.

- STB 2.6-only APIs select lower/upper interpolation indices, sort coordinates with associated indices, construct default L2/L8 records, choose native metadata-record pointers and return Q12 weights/default-use flags.
- Inverse-LUT uniqueness preserves end-of-run selection and reloads the mutable last_index after writes. These operations have no same-signature STB 2.4 conversion counterpart.

### F064: `core/dm_trim_luminance.c`

Classification: native-api-or-table-family-added-removed.

STB 2.6-only luma coordinates use display lookup/L10 fallback or L2 target PQ, scale to the working domain, suppress rounded/upper duplicates and inject/exclude the default before selecting target PQ.

- STB 2.6-only luma coordinates use display lookup/L10 fallback or L2 target PQ, scale to the working domain, suppress rounded/upper duplicates and inject/exclude the default before selecting target PQ.
- The bounded17-entry scratch unions, trap after earlier stores and output index/count order are preserved; STB 2.4 trim reduction is a different entry point and storage contract.

### F065: `core/dm_types.h`

Classification: native LP64/fatal helper representation.

Declare2.6 LP64 scalar views, pair construction and nonreturning bounds/integrity traps.

- The2.6 algorithms use explicit64-bit long/pointer assumptions and fatal paths;2.4 retains typed native layouts and its own failure returns where applicable. The assertions prevent silently building a different data model.

### F066: `core/dm_workspace.h`

Classification: added native workspace representation.

Name2.6 consolidation/output scratch fields and overlapping coefficient/trim/metadata views with exact offset/size assertions.

- Consolidation workspace is 384 bytes and output workspace288 bytes. Reserved regions and overlapping16/32/64-bit union views preserve native stack semantics; they are not removable padding.

### F067: `core/fixed_arithmetic.c`

Classification: native-vendor-algorithm-and-contract.

Saturating add/subtract/abs/normalize primitives and same-bit scalar declarations are common.

- Saturating add/subtract/abs/normalize primitives and same-bit scalar declarations are common. Signed add/subtract/abs/shift result views preserve signed consumers; raw input declarations plus explicit signed views preserve all32/64-bit input patterns. STB 2.6 smpy/sdiv/unorm full-X0 results remain real differences: smpy(0xffffffff,0x7fffffff)=0x1ffffffff, sdiv_sls(1,0xffffffff)=0xffffffffffffffff, and low32-zero unorm returns its original high X0 bits.
- Left shift with negative raw 32 count differs: sshl_ssu(1,-1) gives 0x7fffffff versus 0. Right-shift/negative-shift handling retains the related native behavior.
- STB 2.6 adds saturating sdiv with its separate zero result policy, signed/unsigned 128-bit product outputs and differently defined wide-shift helpers. Output order low-then-high and overlapping output slots are retained.

### F068: `core/fixed_arithmetic.h`

Classification: native-declarations-data-or-private-helper-organization.

Saturating add/subtract/abs/normalize primitives and same-bit scalar declarations are common.

Details shared with: F067.

### F069: `core/fixed_transcendentals.c`

Classification: native-vendor-algorithm-and-contract.

The identical six-stage logarithm polynomial now shares fixed_log_polynomial, alongside the existing fixed_power_polynomial.

- The identical six-stage logarithm polynomial now shares fixed_log_polynomial, alongside the existing fixed_power_polynomial. Taylor log/power, exp and repeated power retain different invalid-scale tests, arithmetic/logical shifts and loop lower bounds despite sharing polynomial/multiply primitives. Same-bit signed/unsigned declarations and the entire square-root core/result are now common after full-domain bounds and 4,106 original cross-version probes; log10Taylor retains its actual full-X0 result (input 0 gives 0x1dbe05bc9 in STB 2.6).
- STB 2.6 repeated-power loop uses (scale-2) modulo32 with do/while semantics; STB 2.4 uses a bounded signed loop. Both use only low32 scale bits, so the former64-bit scale spelling is removed. STB 2.6 adds extended exponent/domain functions, angle interpolation and original bounds traps. Disabled fx_assert now has one common ignored-test/message interface.

### F070: `core/fixed_transcendentals.h`

Classification: native-declarations-data-or-private-helper-organization.

The identical six-stage logarithm polynomial now shares fixed_log_polynomial, alongside the existing fixed_power_polynomial.

Details shared with: F069.

### F071: `core/fixed_transcendentals_tables.h`

Classification: native-declarations-data-or-private-helper-organization.

The identical six-stage logarithm polynomial now shares fixed_log_polynomial, alongside the existing fixed_power_polynomial.

Details shared with: F069.

### F072: `core/host_control.c`

Classification: Dolby version contract, native layout or binding.

STB 2.4 host/core enum adapter accepts -1..3 and maps host SDR2020 6 to core 4, rejecting unsupported ordinary formats before output writes.

Details shared with: C-control-entry.

### F073: `core/host_control.h`

Classification: Dolby version contract, native layout or binding.

Keep the STB 2.4 17-argument/reset/prefix-preservation boundary; 2.6 takes its native host settings.

Details shared with: C-control-entry, C-control-lifecycle.

### F074: `core/kernel_map_composition.c`

Classification: added vendor map composition algorithm.

Compose primary inverse mapping with tone, saturation and chroma maps in 2.6.

- The added ConcatenatePriBwdMapping path copies tone controls, interpolates primary inverse points, calls SMIcore, stores the interpolated inverse-luma plane by narrowing to short without a clamp, then clamps the other three tone/saturation planes to 0..0x2000 and retains signed division/masked index behavior.
- A mapping exceeding512 entries traps after earlier writes; the native inclusive endpoint loop, cached/selected tables and constant-success return remain intact.

### F075: `core/kernel_map_composition.h`

Classification: added vendor entry declaration.

Declare the 2.6 primary inverse/tone/saturation composition entry and its native pointer/result interface.

- No2.4 counterpart is removed; this is part of the extended2.6 tone/map orchestration.

### F076: `core/kernel_map_serialization.c`

Classification: native stream/layout contract with representation cleanup.

Use a common positive 32-bit byte-count result and semantic retained-geometry names; preserve different stream layouts and restore rules.

- 2.4 packs a variable stream up to 10872 bytes with optional secondary/alpha/graphics records and hardware-interface output.2.6 packs a fixed0x15d0-byte stream with changed control/up/downsample/frame fields and indirect output-map storage.
- Loading preserves native dimensions/pitches and selected type/depth fields, clears the specified storage, reconstructs output-map ownership and restores output dimensions from primary input where required. Native copy/store order is retained.
- All produced lengths are nonnegative and fit 32 bits; the unified unsigned-int result preserves every valid return bit pattern, including the former64-bit 0x15d0 return's zero upper bits.

### F077: `core/kernel_map_serialization.h`

Classification: native stream/buffer contract.

Share pointer and positive byte-count types while documenting version-specific stream sizes, geometry restore behavior and required disjoint backing.

- 2.4 exposes hardware-interface packing and variable stream extent;2.6 uses fixed0x15d0 bytes and its own kernel/output-map layout. Both retain explicit unaligned little-endian access.

### F078: `core/matrix_arithmetic.c`

Classification: native-vendor-algorithm-and-contract.

Precision/narrowing shares matrix_scale_impl.h; precision is bounded0..64 and now has the same unsigned 32 declaration/body.

- Precision/narrowing shares matrix_scale_impl.h; precision is bounded0..64 and now has the same unsigned 32 declaration/body. General narrowing retains signed 16 versus raw 32 scale/result domains and truncate versus saturate policies. STB 2.6 additionally exposes rounded narrowing.
- InvM33 delegates to an identical common inverse core in matrix_scale_impl.h, preserving raw determinant/cofactor arithmetic, explicit low16 scale interpretation and all input reads before output stores. Inputs now share raw 32 declarations because high16 bits are ignored in both versions. Native signed 16 versus raw 32 singular results (-1 versus 65535) and final truncate versus saturate packing remain different.
- Original/published/new inverse checks cover1024 matrices pertarget including singular, signed 16/raw 32 scale extremes, unaligned and overlapping input/output. STB 2.4 alone exposes M33TimesM33 with its fixed operand access widths and scale result.

### F079: `core/matrix_arithmetic.h`

Classification: native-declarations-data-or-private-helper-organization.

Precision/narrowing shares matrix_scale_impl.h; precision is bounded0..64 and now has the same unsigned 32 declaration/body.

Details shared with: F078.

### F080: `core/metadata_init_body.c`

Classification: retained vendor API/layout/algorithm delta.

Per-version initialization owns genuinely different singleton/allocation lifetimes and error cleanup; shared config fields now have semantic names.

- P01
- P02
- P07

### F081: `core/metadata_runtime.c`

Classification: retained vendor API/layout/algorithm delta.

Native host process/reset/release signatures and sequencing remain distinct; equivalent trailing-zero arithmetic now uses a shared wrapping scan.

Details shared with: F080.

### F082: `core/module_parser.h`

Classification: retained vendor API/layout/algorithm delta.

Host callback interfaces, ownership, allocation imports and release contracts remain native; logger typedef is common.

Details shared with: F080.

### F083: `core/module_parser_context.h`

Classification: retained vendor API/layout/algorithm delta.

Native application layouts remain0x780 singleton plus contiguous backing versus 0x770 allocated context with per-instance sizes; common config layout is asserted.

Details shared with: F080.

### F084: `core/numeric_color_matrix.c`

Classification: native-vendor-algorithm-and-contract.

YUV-to-RGB offset calculation now has an identical explicit-width common body, including unaligned reads, signed 64 arithmetic shift and row-by-row reload across overlapping output.

- YUV-to-RGB offset calculation now has an identical explicit-width common body, including unaligned reads, signed 64 arithmetic shift and row-by-row reload across overlapping output.
- Definition/custom-primary XYZ/LMS transforms retain coefficient-table layout, determinant truncation/masks and fixed XYZ-to-LMS operand differences. Cross-talk/white-point composition differs in intermediate rounding, division order, row/column scaling and saturation.
- STB 2.6 adds DE_CT/APPLY_CT and reapplication helpers, V8-primary wrappers and diagonal getters. Its Lms2RgbCtWp boundary reserves an extra64-bit argument slot before the white-point pointer; that slot is intentionally retained.
- Rgb2LmsWpByDefV3 now explicitly returns its bounded native scale in both versions. The original STB 2.6 epilogue preserves W0 after GetRgb2LmsM33; the former void declaration was an unsupported decompiler choice, removed using instruction and native-return evidence.
- GetLms2RgbByDefM33Fxp(definition7) is an explicit result-domain difference: original STB 2.4 returns signed 16 value32, while original STB 2.6 returns raw 32 value0xffff0020. GetRgb2LmsByDefM33 and forward custom-primary scale results are bounded positive and now share unsigned 32 declarations; inverse getters retain their meaningful narrowed result domains.

### F085: `core/numeric_color_matrix.h`

Classification: native-declarations-data-or-private-helper-organization.

YUV-to-RGB offset calculation now has an identical explicit-width common body, including unaligned reads, signed 64 arithmetic shift and row-by-row reload across overlapping output.

Details shared with: F084.

### F086: `core/numeric_helpers.c`

Classification: native-vendor-algorithm-and-contract.

Range fallback and exceptional depths differ natively: GetRange(10,3) produces 0/1023 versus 64/959.

- Range fallback and exceptional depths differ natively: GetRange(10,3) produces 0/1023 versus 64/959. Minimum/extent alias reload semantics are retained.
- Bypass offset stores now use the common YUV helper; each version retains divisor, signed-depth and zero/full-range rules. CommitKsUs has a different native input interface and output-store sequence.
- STB 2.6 adds level-bit operations, stable ascending sort, downscale/filter setup, inverse/interpolated LUT helpers and display/primary pointer lookups. Their signed comparisons, last-index domains and output ordering remain explicit.

### F087: `core/numeric_helpers.h`

Classification: native-declarations-data-or-private-helper-organization.

Range fallback and exceptional depths differ natively: GetRange(10,3) produces 0/1023 versus 64/959.

Details shared with: F086.

### F088: `core/numeric_tables.h`

Classification: native-declarations-data-or-private-helper-organization.

STB 2.6 table aliases and XYZ-definition/white-point-CCT constants bind its custom-primary and color-temperature implementation.

- STB 2.6 table aliases and XYZ-definition/white-point-CCT constants bind its custom-primary and color-temperature implementation. Values and storage ownership were preserved; no matching STB 2.4 standalone header exists.

### F089: `core/numeric_tone.c`

Classification: native-api-or-table-family-added-removed.

STB 2.6-only tone/saturation forward/backward wrappers select parameter block0x1650 and native Q11 domain conversions.

- STB 2.6-only tone/saturation forward/backward wrappers select parameter block0x1650 and native Q11 domain conversions. Reference/value parameter names now match declarations and exact underlying types replace opaque aliases.
- HLG OOTF uses extended PQ/power helpers, including native repeated loads and zero/negative-ratio behavior. These are additional vendor API bodies, not alternate spellings of STB 2.4 vector-map helpers.

### F090: `core/numeric_tone.h`

Classification: native-declarations-data-or-private-helper-organization.

STB 2.6-only tone/saturation forward/backward wrappers select parameter block0x1650 and native Q11 domain conversions.

Details shared with: F089.

### F091: `core/numeric_transfer.c`

Classification: native-vendor-algorithm-and-contract.

GenerateL2GLut common sampling/store structure matches and both now take unsigned 32 gamma: all original STB 2.6 gamma uses explicitly discard high32 bits.

- GenerateL2GLut common sampling/store structure matches and both now take unsigned 32 gamma: all original STB 2.6 gamma uses explicitly discard high32 bits. The fatal breakpoint check and zero-extended(STB 2.4)/sign-extended(STB 2.6) slope denominator remain real behavior differences.
- STB 2.4 additionally implements97-point target-EOTF LUT/lerp and gamma alpha/beta calculation. STB 2.6 instead implements CCT/xy/XYZ/ICtCp conversion, intended white-point CCT and native gamma-parameter preparation with its own state writes.

### F092: `core/numeric_transfer.h`

Classification: native-declarations-data-or-private-helper-organization.

GenerateL2GLut common sampling/store structure matches and both now take unsigned 32 gamma: all original STB 2.6 gamma uses explicitly discard high32 bits.

Details shared with: F091.

### F093: `core/numeric_transfer_tables.h`

Classification: native-declarations-data-or-private-helper-organization.

STB 2.4-only target-EOTF/logarithmic gamma tables belong to its97-point helper family.

- STB 2.4-only target-EOTF/logarithmic gamma tables belong to its97-point helper family. Gamma breakpoint, pulsar and SDR values already share common include files across both versions; raw values remain unchanged.

### F094: `core/original_data.c`

Classification: Dolby version contract, native layout or binding.

Canonical 2.6 BSS/signature/table objects and native version/diagnostics, with all member sizes/offsets asserted.

Details shared with: C-data-ownership.

### F095: `core/parser_contract.h`

Classification: retained vendor API/layout/algorithm delta.

Every differing selector names an actual library policy or diagnostic value; cosmetic alignment/one-bit helper selection removed.

- P03
- P04
- P05
- P08
- P09
- P10
Details shared with: F080, F037.

### F096: `core/parser_info.c`

Classification: retained vendor API/layout/algorithm delta.

API text changes0.4.1->0.4.3; native contiguous-versus-separate string storage and equal algorithm/build getters retained.

Details shared with: F080.

### F097: `core/pq12_tables.h`

Classification: native-declarations-data-or-private-helper-organization.

Identical decoder PQ table values bind to separate named arrays in STB 2.4 and the retained read-only constant aggregate in STB 2.6.

- Identical decoder PQ table values bind to separate named arrays in STB 2.4 and the retained read-only constant aggregate in STB 2.6. The conversion changes aliases/storage owner, not the lookup values.

### F098: `core/pq_conversion.c`

Classification: native-vendor-algorithm-and-contract.

Common12/16-bit inverse interpolation bodies use pq12_impl.h and now share unsigned 32 input declarations because both explicitly mask low16 bits.

- Common12/16-bit inverse interpolation bodies use pq12_impl.h and now share unsigned 32 input declarations because both explicitly mask low16 bits. LToPQ12 now has one bounded unsigned 32 result declaration; STB 2.4 still permits rounded4096 while STB 2.6 caps4095. Both decoder conversions cap4095.
- Signed/extended L2PqX retains native range branches, rounding and meaningful return widths: raw 64 input 0xffffffffffffffff gives signed 32 -108 in STB 2.4 versus full64 0x7ffffffffff94 in STB 2.6. STB 2.6 adds16/18-bit LUT conversions, signed forward/inverse extended PQ and table-interpolated extended conversions with original fatal bounds.
- The private STB 2.6 asr64 implementation was replaced by the shared fixed_asr64 for its existing constant1..63 shift callers; duplicate shift presentation is removed without changing their results.

### F099: `core/pq_conversion.h`

Classification: native-declarations-data-or-private-helper-organization.

Common12/16-bit inverse interpolation bodies use pq12_impl.h and now share unsigned 32 input declarations because both explicitly mask low16 bits.

Details shared with: F098.

### F100: `core/pq_conversion_tables.h`

Classification: native-declarations-data-or-private-helper-organization.

Common12/16-bit inverse interpolation bodies use pq12_impl.h and now share unsigned 32 input declarations because both explicitly mask low16 bits.

Details shared with: F098.

### F101: `core/register_commit.c`

Classification: Dolby version contract, native layout or binding.

Retain singleton versus multi-input commit/packet/source/target sequencing and native register maps; extracted composer/LUT algorithms match, source/packet/HDR/packing temporary lifetimes are named.

Details shared with: C-control-entry, C-composer, C-lut, C-vsvdb, C-metadata-storage, C-transport-content, C-graphics, C-publication.

### F102: `core/register_commit.h`

Classification: Dolby version contract, native layout or binding.

Native commit inputs differ in arity/layout, while every buffer parameter uses a pointer type.

Details shared with: C-control-entry, C-composer, C-publication.

### F103: `core/resampling_filters.c`

Classification: native-vendor-algorithm-and-contract.

Read-log decoding and coefficient sign extension now use matching representation; equivalent signed counter>=2/unsigned log-level>=3 comparisons and required64-bit-offset checks also match.

- Read-log decoding and coefficient sign extension now use matching representation; equivalent signed counter>=2/unsigned log-level>=3 comparisons and required64-bit-offset checks also match.
- Component/direction declarations now share raw 32 inputs with explicit signed(STB 2.4)/unsigned(STB 2.6) views inside their different native address arithmetic. Diagnostics capture timing, prediction-flag/delta reloads and cached versus reloaded payload owners remain native.
- STB 2.4 additionally exposes the spatial filter/phase reader; STB 2.6 has no same original exported body in this unit. Its removal is an API-family difference, not a reason to remove the working STB 2.4 implementation.

### F104: `core/resampling_filters.h`

Classification: native-declarations-data-or-private-helper-organization.

Read-log decoding and coefficient sign extension now use matching representation; equivalent signed counter>=2/unsigned log-level>=3 comparisons and required64-bit-offset checks also match.

Details shared with: F103.

### F105: `core/shared_data.h`

Classification: Dolby version contract, native layout or binding.

Canonical 2.6 shared objects prevent independent mutable/table mirrors.

Details shared with: C-data-ownership.

### F106: `core/tone_controls.c`

Classification: native-vendor-algorithm-and-contract.

STB 2.4 updates level-adjustment controls, maps tone control and derives target/HLG signal fields through small singleton helpers.

- STB 2.4 updates level-adjustment controls, maps tone control and derives target/HLG signal fields through small singleton helpers. STB 2.6 prepares indexed source/target color state, metadata overrides, adaptive backlight, PQ bounds and tone-LUT sizes in a distinct consolidation path.
- Native context offsets, secondary-signal propagation, repeated reads after packed stores, LUT bound traps and per-signal update ordering remain version-specific. Audited role names now describe the local bindings, including their multiple lifetimes; existing native object types and offsets remain separate recovery work.

### F107: `core/tone_controls.h`

Classification: native-declarations-data-or-private-helper-organization.

STB 2.4 updates level-adjustment controls, maps tone control and derives target/HLG signal fields through small singleton helpers.

Details shared with: F106.

### F108: `core/tone_curve.c`

Classification: native-vendor-algorithm-and-contract.

STB 2.4 exposes2/3-point generation, source/display parameter objects and a 160-byte comparison of 168-byte control.

- STB 2.4 exposes2/3-point generation, source/display parameter objects and a 160-byte comparison of 168-byte control. Ratio-pointer selection can leave three auxiliary maps untouched; mode3 two-point calls make no output writes.
- STB 2.6 coordinator compares 0x1a0 control and 0xf8 parameter bytes at 0x1650, prepares/builds/packs, stores first sample into four slots and returns native change flags; its mode/context slots remain part of the call boundary.

### F109: `core/tone_curve.h`

Classification: native-declarations-data-or-private-helper-organization.

STB 2.4 exposes2/3-point generation, source/display parameter objects and a 160-byte comparison of 168-byte control.

Details shared with: F108.

### F110: `core/tone_curve_lut.c`

Classification: native-vendor-algorithm-and-contract.

STB 2.4 samples a 4096-step log-coordinate curve, optionally interpolates into 4096 outputs from a padded515-element work buffer and retains its finite size/untouched-cell rules.

- STB 2.4 samples a 4096-step log-coordinate curve, optionally interpolates into 4096 outputs from a padded515-element work buffer and retains its finite size/untouched-cell rules.
- STB 2.6 samples512 primary/identity values, applies HLG where required and packs register-ready output plus control state with native reloads. The algorithm and buffer layouts differ, not just their names.

### F111: `core/tone_curve_lut.h`

Classification: native-declarations-data-or-private-helper-organization.

STB 2.4 samples a 4096-step log-coordinate curve, optionally interpolates into 4096 outputs from a padded515-element work buffer and retains its finite size/untouched-cell rules.

Details shared with: F110.

### F112: `core/tone_curve_lut_table.inc`

Classification: native-api-or-table-family-added-removed.

STB 2.4-only 4096-element log-coordinate table feeds its native curve sampler.

- STB 2.4-only 4096-element log-coordinate table feeds its native curve sampler. STB 2.6 uses different tone-core sampling and does not need this table; its raw values were preserved.

### F113: `core/tone_mapping_math.c`

Classification: native-api-or-table-family-added-removed.

STB 2.6-only tone/saturation primitives implement local-saturation patching, HLG EOTF parameter generation, Hermite-based tone/saturation curves and cutoff scoring with native fixed-point scales and 128-bit product reduction.

- STB 2.6-only tone/saturation primitives implement local-saturation patching, HLG EOTF parameter generation, Hermite-based tone/saturation curves and cutoff scoring with native fixed-point scales and 128-bit product reduction.
- The HLG stage writes256 words at output+0x30 and can trap after partial writes; TMScore's optional sqrt-output store precedes later config reads. Aliasing, saturation bounds and native BRK behavior are preserved.

### F114: `core/tone_mapping_math.h`

Classification: native-declarations-data-or-private-helper-organization.

STB 2.6-only tone/saturation primitives implement local-saturation patching, HLG EOTF parameter generation, Hermite-based tone/saturation curves and cutoff scoring with native fixed-point scales and 128-bit product reduction.

Details shared with: F113.

### F115: `core/tone_mapping_math_tables.h`

Classification: native-declarations-data-or-private-helper-organization.

STB 2.6-only tone/saturation primitives implement local-saturation patching, HLG EOTF parameter generation, Hermite-based tone/saturation curves and cutoff scoring with native fixed-point scales and 128-bit product reduction.

Details shared with: F113.

### F116: `core/tone_maps.c`

Classification: native-vendor-algorithm-and-contract.

STB 2.4 vector maps generate independent intensity/saturation LUTs from curve pairs and display/ratio controls.

- STB 2.4 vector maps generate independent intensity/saturation LUTs from curve pairs and display/ratio controls. STB 2.6 builds primary plus three auxiliary maps from parameter blocks and tone/saturation core calls.
- Native mode dispatch, square-root scratch slots, clamping/rounding constants,511/512 endpoints, packed output offsets and repeated loads distinguish the bodies. Audited role names now describe the local bindings and their multiple lifetimes; the distinct parameter layouts and native object typing remain explicit.

### F117: `core/tone_maps.h`

Classification: native-declarations-data-or-private-helper-organization.

STB 2.4 vector maps generate independent intensity/saturation LUTs from curve pairs and display/ratio controls.

Details shared with: F116.

### F118: `core/tone_parameters.c`

Classification: native-vendor-algorithm-and-contract.

STB 2.4 derives2/3-point mapped parameters and curve segments from source/display structures with ordered output aliases.

- STB 2.4 derives2/3-point mapped parameters and curve segments from source/display structures with ordered output aliases. STB 2.6 copies a different control block into its parameter record, computes mode/HLG/native-luminance transforms and stores mapped low/mid/high fields.
- Native shifts, source clamps, white-point/trim scales, denominator handling and parameter destinations remain distinct; no algebraic unification was attempted without original-instruction evidence.

### F119: `core/tone_parameters.h`

Classification: native-declarations-data-or-private-helper-organization.

STB 2.4 derives2/3-point mapped parameters and curve segments from source/display structures with ordered output aliases.

Details shared with: F118.

### F120: `core/tone_tables.inc`

Classification: native-api-or-table-family-added-removed.

STB 2.4 table owners bind the already-common tone log10/rate/power include values.

- STB 2.4 table owners bind the already-common tone log10/rate/power include values. STB 2.6 obtains corresponding arrays from its constant aggregate; values are shared while ownership remains native.

### F121: `core/trim_reduction.c`

Classification: native-api-or-table-family-added-removed.

STB 2.4-only reduction reads packed big-endian metadata trim records, compacts L2/L8/L10 levels, sorts and selects target/display brackets before copying native records.

- STB 2.4-only reduction reads packed big-endian metadata trim records, compacts L2/L8/L10 levels, sorts and selects target/display brackets before copying native records.
- Equal keys swap and can reverse duplicate-key ordering; fixed arrays hold16 records. STB 2.6 replaces this entry-point family with separate selection/default/interpolation builders, not the same reduction contract.

### F122: `core/trim_reduction.h`

Classification: native-declarations-data-or-private-helper-organization.

STB 2.4-only reduction reads packed big-endian metadata trim records, compacts L2/L8/L10 levels, sorts and selects target/display brackets before copying native records.

Details shared with: F121.

### F123: `core/trim_tables.inc`

Classification: native-api-or-table-family-added-removed.

STB 2.4-only display and primary lookup records support trim reduction, while STB 2.6 uses its own display storage and 19-primary table.

- STB 2.4-only display and primary lookup records support trim reduction, while STB 2.6 uses its own display storage and 19-primary table. Native initializer values and ownership were preserved.

### F124: `core_objects.mk`

Classification: build/source organization.

Build all included C units for each library and share byte alignment as an ordinary source object.

- 2.4 retains single-input host/control orchestration and legacy trim helpers.2.6 adds backlight, output metadata, interpolation/luminance trim and primary-map composition units, and removes superseded single-input runtime/metadata/graphics units.
- The restored common align_to_byte object replaces equivalent inline code in 2.6; all module algorithms remain source-compiled.

### F125: `host_abi.h`

Classification: host ABI assertions.

Check the actual4.9 T4c and 5.15 CoreELEC callback/settings/nested payload layouts against matching host headers.

- 4.9 callback table 48 bytes and single settings/output ownership differ from the 5.15 table 112 bytes,0x4f58 multi-input settings, three0x68-byte input records and two Core1 outputs.
- Parser input-kind enums, signal values, register/LUT/HDR/extension fields and offsets retain native host semantics; assertions fail compilation for a mismatched provider.

### F126: `host_adapter.c`

Classification: host adapter/lifetime/identity behavior.

Adapt exact host callback prototypes, module startup and registration failure policy for each host.

- 4.9 provides vprintk-backed logging, singleton parser calls and the many-argument control adapter with host/core format translation.5.15 passes explicit contexts and input kind, invokes multi-control settings, builds the version/chip strings, and registers multi callbacks.
- 4.9 propagates configure/registration failures.5.15 records the registration result in a read-only parameter, logs failure/success and returns successful insertion to keep possibly retained callbacks resident; replacing the attempted registration requires reboot.

### F127: `host_adapter.h`

Classification: module metadata identity.

Retain each reconstructed module's selected description, license declaration and reconstruction version.

- 2.4 declares GPL/version0.1 and 2.6 declares AML/version0.2 with matching descriptions. These are retained module metadata declarations, not inferred kernel semantics or a legal relicensing decision.

### F128: `runtime_bridge.c`

Classification: kernel service integration.

Supply each core's actual kernel logging, CPU, allocation, clear/free and diagnostics service signatures.

- 4.9 configures copied control-runtime services and printk/vprintk adapters.5.15 supplies vmalloc/vfree-backed parser ownership, OS memset and _printk through its module/control service objects.
- Kernel-provided symbol names, CRCs and instrumentation are rebuilt from the selected generated inputs; service tables remain separate from algorithm/state ownership.

### F129: `runtime_bridge.h`

Classification: removed native runtime-setup interface.

Remove the single-input runtime-configure declaration when2.6 uses statically initialized kernel service objects.

- The2.6 host adapter does not call the 2.4 configuration hook; kernel services are bound in runtime_bridge.c before registration instead.

## Validation and remaining work

Validation combines matching-kernel source builds, host-layout/CRC checks,
original-instruction comparisons and published/new linked-module native
differentials. Cases cover numerical boundaries, unaligned/disjoint backing,
audited overlapping views, parser lifecycle/streams, native traps, observer
callbacks and partial output. Only the recorded OS/observer services are
modeled; algorithm calls execute AArch64 instructions.

Finite native comparisons and source review do not establish exhaustive
malformed-input equivalence or hardware playback. The 5.15 targets share
algorithm/data sections and source; kernel metadata and symbol CRCs differ.
Direct STB 2.6 callbacks require a non-CFI host configuration.

Next work is matching-device load/playback validation (FEL/MEL, output modes,
seeking/reset and performance), broader coverage of remaining native
numerical/metadata edge paths, and native object typing for distinct
STB 2.6 algorithms. The hunk ledger distinguishes that readability work
from proven equivalent-operation cleanup. Any intentional hardening or
behavior change must receive its own changelog entry and validation.
