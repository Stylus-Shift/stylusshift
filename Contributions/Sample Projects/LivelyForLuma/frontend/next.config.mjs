/** @type {import('next').NextConfig} */
export default {
  experimental: {
    optimizePackageImports: ["@chakra-ui/react"],
  },
  eslint: {
    ignoreDuringBuilds: true,
  },
};
