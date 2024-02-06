#pragma once

// IMPORT THIRD-PARTY LIBRARIES
#include <pxr/usd/ar/defaultResolver.h>

PXR_NAMESPACE_OPEN_SCOPE

class URIResolver : public ArDefaultResolver {
public:
    URIResolver();
    ~URIResolver();

    ArResolvedPath _Resolve(const std::string& path) const override;
};

PXR_NAMESPACE_CLOSE_SCOPE
