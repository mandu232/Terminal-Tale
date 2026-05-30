#include "GetCharWidth.h"

// ─────────────────────────────────────────────────────────────
//  GetConsoleCharWidth
//
//  Windows 콘솔에서 실제로 2셀 폭을 차지하는 문자 범위만 2를 반환.
//  0x1100 이상 전체를 2로 처리하던 방식은 em-dash(0x2014), 박스문자
//  등 비-CJK 유니코드까지 2칸으로 오계산해 우측 테두리 정렬이 틀어짐.
// ─────────────────────────────────────────────────────────────
int GetConsoleCharWidth(wchar_t c)
{
    // Hangul Jamo (0x1100–0x11FF)
    if (c >= 0x1100 && c <= 0x11FF) return 2;
    // Hangul Jamo Extended-A (0xA960–0xA97F)
    if (c >= 0xA960 && c <= 0xA97F) return 2;
    // Hangul Compatibility Jamo (0x3130–0x318F)
    if (c >= 0x3130 && c <= 0x318F) return 2;
    // Hangul Syllables (0xAC00–0xD7A3)
    if (c >= 0xAC00 && c <= 0xD7A3) return 2;
    // Hangul Jamo Extended-B (0xD7B0–0xD7FF)
    if (c >= 0xD7B0 && c <= 0xD7FF) return 2;
    // CJK Unified Ideographs Extension A (0x3400–0x4DBF)
    if (c >= 0x3400 && c <= 0x4DBF) return 2;
    // CJK Unified Ideographs (0x4E00–0x9FFF)
    if (c >= 0x4E00 && c <= 0x9FFF) return 2;
    // CJK Compatibility Ideographs (0xF900–0xFAFF)
    if (c >= 0xF900 && c <= 0xFAFF) return 2;
    // CJK Radicals / Kangxi Radicals (0x2E80–0x2FDF)
    if (c >= 0x2E80 && c <= 0x2FDF) return 2;
    // CJK Symbols and Punctuation (0x3000–0x303F)
    if (c >= 0x3000 && c <= 0x303F) return 2;
    // Hiragana (0x3040–0x309F)
    if (c >= 0x3040 && c <= 0x309F) return 2;
    // Katakana (0x30A0–0x30FF)
    if (c >= 0x30A0 && c <= 0x30FF) return 2;
    // Enclosed CJK / CJK Compatibility (0x3200–0x33FF)
    if (c >= 0x3200 && c <= 0x33FF) return 2;
    // CJK Compatibility Forms (0xFE30–0xFE4F)
    if (c >= 0xFE30 && c <= 0xFE4F) return 2;
    // Fullwidth Forms (0xFF01–0xFF60, 0xFFE0–0xFFE6)
    if (c >= 0xFF01 && c <= 0xFF60) return 2;
    if (c >= 0xFFE0 && c <= 0xFFE6) return 2;
    return 1;
}